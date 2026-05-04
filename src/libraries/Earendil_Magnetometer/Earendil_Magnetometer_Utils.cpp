#include <Earendil_Magnetometer.h>  // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Magnetometer {

    Adafruit_MMC5603 magnetometer = Adafruit_MMC5603();
    
    const float FILTER_COEFF    = 1; //low pass filter coefficient
    const int SAMPLE_DELAY      = 200; // 200ms between samples

    struct Magmeter {
        float hard_min[3]   = {9999, 9999, 9999}; //min raw values for hard iron calibration
        float hard_max[3]   = {-9999, -9999, -9999}; //max raw values for hard iron calibration
        float hard_iron[3]  = {0.61, 5.95, -36.13}; //finall hard iron offset values
        float filtered[3]   = {0.0, 0.0, 0.0}; //this is the filtered magnetometer values
        bool filter_initted = false; //flag to make sure the filter isnt being biased too low
    };

    Magmeter magmeter;

    const float SOFT_IRON[3][3] = //soft iron hard coded values
    {
        {1.010, -0.01, 0.011},
        {-0.001, 0.996, 0.007},
        {0.011, 0.007, 0.995}
    };
    void setup(){
        setupMagnetometer();
    }

    void setupMagnetometer(){
        magnetometer.begin(MMC56X3_DEFAULT_ADDRESS, &Wire);
    }

    double getHeading(double calibrated[3], float ax, float ay, float az) {
        // Derive pitch and roll from the gravity vector so the mag vector
        // can be rotated into the horizontal plane before computing heading.
        float roll  = atan2f(ay, az);
        float pitch = atan2f(-ax, sqrtf(ay*ay + az*az));

        float mx_h = (float)calibrated[0] * cosf(pitch)
                   + (float)calibrated[2] * sinf(pitch);
        float my_h = (float)calibrated[0] * sinf(roll) * sinf(pitch)
                   + (float)calibrated[1] * cosf(roll)
                   - (float)calibrated[2] * sinf(roll) * cosf(pitch);

        return (double)(atan2f(-my_h, mx_h) * 180.0f / (float)M_PI);
    }

    void applyCalibration(double* calibrated){
        double temp[3];

        for (int i = 0; i < 3; i++) //removes the hard iron
        {
            temp[i] = magmeter.filtered[i] - magmeter.hard_iron[i];
        }

        for (int i = 0; i < 3; i++) //get the soft iron calibrateion
        {
            calibrated[i] = 
            SOFT_IRON[i][0] * temp[0] +
            SOFT_IRON[i][1] * temp[1] +
            SOFT_IRON[i][2] * temp[2];
        }

    }

    void updateFilter(double raw[3]){  // pass in the raw data into the filter updater
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

    void updateHardIron(double raw[3]){  //updates the hard iron values
        for (int i = 0; i < 3; i++){
            if (raw[i] < magmeter.hard_min[i]) magmeter.hard_min[i] = raw[i]; // if the raw values are smaller, then replace the hard min
            if (raw[i] > magmeter.hard_max[i]) magmeter.hard_max[i] = raw[i]; // if the raw values are larger replace the hard maximum
        }
    }
    
    void calibrateMagnetometer(){
        constexpr uint32_t NUMBER_SAMPLES = 100;
        constexpr uint32_t SAMPLING_RATE  = 50;

        magmeter.filter_initted = false; //reset the magnetometer filter so that way its not holding old values
        for (int i = 0; i < 3; i++) {
            magmeter.filtered[i] = 0;
        }

        for (int i = 0; i < 3; i++) { //reset the hard iron components
          magmeter.hard_min[i] = 9999;
          magmeter.hard_max[i] = -9999;
        }

        for (int i = 0; i < NUMBER_SAMPLES; i++){  //while the current timer is less than the calibration timer
            sensors_event_t event; //init a magnetometer sensor event
            magnetometer.getEvent(&event); //this grabs the current magnetometer readings

            double raw[3] = {event.magnetic.x, event.magnetic.y, event.magnetic.z};//this gets the raw x, y, and z values
            updateFilter(raw);
            updateHardIron(raw);
            vTaskDelay(pdMS_TO_TICKS(SAMPLING_RATE));//delay by the sample delay to let the user move the magnetometer to a new position
          }
          for (int i = 0; i < 3; i++)
          {
          magmeter.hard_iron[i] = (magmeter.hard_max[i] + magmeter.hard_min[i]) / 2.0;
          }
    }
}
