#include <Earendil_Magnetometer.h>  // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Magnetometer {

  Adafruit_MMC5603 magnetometer = Adafruit_MMC5603();

  void setup(){
    setupMagnetometer();
  }

  void setupMagnetometer(){
    magnetometer.begin(MMC56X3_DEFAULT_ADDRESS, &Wire);
  }
  
  float getHeading(float calibrated[3]){
    float heading = (-1) * atan2(calibrated[0], calibrated[1]) * 180.0 / M_PI; //get the heading in degrees
    //if (heading < 0) heading += 360;  // if negative, add 360 to keeep the value positive
    return heading;                   // return the heading angle
  }
  
  const float FILTER_COEFF = 10.0; //low pass filter coefficient
  const int CALIBRATION_TIME = 10000; // 10 second Calibration time
  const int SAMPLE_DELAY = 200; // 200ms between samples

  struct Magmeter {
    float hard_min[3] = {9999, 9999, 9999}; //min raw values for hard iron calibration
    float hard_max[3] = {-9999, -9999, -9999}; //max raw values for hard iron calibration
    float hard_iron[3] = {0.0, 0.0, 0.0}; //finall hard iron offset values
    float scale[3] = {1.0, 1.0, 1.0}; //this is the soft iron scaling factor
    float filtered[3] = {0.0, 0.0, 0.0}; //this is the filtered magnetometer values
    bool filter_initted = false; //flag to make sure the filter isnt being biased too low
  };

  Magmeter magmeter;

  void updateFilter(float raw[3]){  // pass in the raw data into the filter updater
    if (!magmeter.filter_initted){  // if the filter has not been initialized, theres no reference values to go off of yet
      for (int i = 0; i < 3; i++){
        magmeter.filtered[i] = raw[i]; //place in the raw value into the filter to give a reference value
      }
      magmeter.filter_initted = true; //set the value to true so its initial setup isnt needed anymore
    }

    for (int i = 0; i < 3; i++){ //pass through the xyz values
      magmeter.filtered[i] = (magmeter.filtered[i] * FILTER_COEFF + raw[i]) / (FILTER_COEFF + 1.0); //filter the raw values using the filtering used for signals
    }
  }

  void updateHardIron(){  //updates the hard iron values
    for (int i = 0; i < 3; i++){
      float val = magmeter.filtered[i];
      if (val < magmeter.hard_min[i]) magmeter.hard_min[i] = val; // if the raw values are smaller, then replace the hard min
      if (val > magmeter.hard_max[i]) magmeter.hard_max[i] = val; // if the raw values are larger replace the hard maximum
    }
  }

  void computeSoftIron(){
    float radius[3]; //array to hold the x,y,z radius
    for (int i = 0; i < 3; i++){
      radius[i] = (magmeter.hard_max[i] - magmeter.hard_min[i]) / 2.0; //use the hard max and min and divide by 2 to find the radius
    }

    float avg_radius = (radius[0] + radius[1] + radius[2]) / 3.0; //average out the x, y and z radii

    for (int i = 0; i < 3; i++){
      if (radius[i] != 0) magmeter.scale[i] = avg_radius / radius[i]; //change out the radius scale if its not 0 to prevent the value from killing the scale factor
    }
  }

  void finalizeCalibration(){
    for (int i = 0; i < 3; i++){
      magmeter.hard_iron[i] = (magmeter.hard_max[i] + magmeter.hard_min[i]) / 2.0;  //Computes the hard iron calibration by averaging the max and min to find the midpoint between the two
    }
    computeSoftIron(); //call the soft iron calibrator
  }

  void calibrateMagnetometer(){
    Serial.println("rotate sensor in all directions"); //directions for the user to calibrate the magnetometer
    unsigned long start = millis(); //start timer in milliseconds for the timer
    
    while (millis() - start < CALIBRATION_TIME){  //while the current timer is less than the calibration timer
    
      sensors_event_t event; //init a magnetometer sensor event
      magnetometer.getEvent(&event); //this grabs the current magnetometer readings

      float raw[3] = {event.magnetic.x, event.magnetic.y, event.magnetic.z};//this gets the raw x, y, and z values
      updateFilter(raw);//update the filter for this next set of raw data
      updateHardIron(
        
      ); //update the soft iron calibration shift
      delay(SAMPLE_DELAY);//delay by the sample delay to let the user move the magnetometer to a new position
    }
    Serial.println("\nCalibration Done");
    finalizeCalibration(); //get the hard iron and soft iron calibration based on the data recieved
  }

  void applyCalibration(float calibrated[3]){
    for (int i = 0; i < 3; i++){
      float no_hard = magmeter.filtered[i] - magmeter.hard_iron[i]; //subtract by the hard iron calibration scale
      calibrated[i] = no_hard * magmeter.scale[i];//scale by the soft iron calibration factor
    }
  }
  
  void printData(float heading){
    Serial.print("Heading: "); //label for the heading
    Serial.println(heading, 2); //prints out the actual heading
  }


}
