#include <Wire.h>
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>

Adafruit_MMC5603 mag;

struct Magmeter{
  public:
    float hard_min[3] = {9999, 9999, 9999}; //ensures its crazy out of scope from any actual values so it can be filled
    float hard_max[3] = {-9999, -9999, -9999};
    float hard_iron[3] = {0.0, 0.0, 0.0};

    float soft_min[3] = {999, 999, 999};
    float soft_max[3] = {-999, -999, -999};
    float soft_iron[3][3];

};

Magmeter magmeter;

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println("Starting I2C scan...");

  // Just initialize I2C — default pins 2(SDA)/3(SCL) will be used
  Wire.begin();

  // I2C scan
  byte error, address;
  int nDevices = 0;

  for(address = 1; address < 127; address++ ) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
    } 
    else if (error == 4) {
      Serial.print("Unknown error at 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
    }
  }

  if (nDevices == 0) Serial.println("No I2C devices found.");
  else Serial.println("I2C scan complete.");

  // Initialize MMC5603
  Serial.println("Initializing MMC5603...");
  if (!mag.begin(0x30, &Wire)) {
    Serial.println("MMC5603 not detected!");
    while (1);
  }
  Serial.println("MMC5603 detected!");
}

void loop() {
  sensors_event_t event;
  mag.getEvent(&event);

  float mag_data[3] = {event.magnetic.x, event.magnetic.y, event.magnetic.z};
//===============HARD IRON CALIBRATION=====================//
  //cehcks if the raw data is larger or smaller than the min or max
  for (int i = 0; i < 3; i++)
  {
      if (mag_data[i] < magmeter.hard_min[i])
      {
        magmeter.hard_min[i] = mag_data[i];
      }


      if (mag_data[i] > magmeter.hard_max[i])
      {
        magmeter.hard_max[i] = mag_data[i];
      }
  }
//save the final hard iron offset value
  for (int i = 0; i < 3; i++) {
      magmeter.hard_iron[i] = (magmeter.hard_max[i] + magmeter.hard_min[i]) / 2;
  }





//================SOFT IRON CALIBRATION======================//
float no_hard_iron[3];
//remove the hard iron offset
  for (int i = 0; i < 3; i++) {
      no_hard_iron[i]= mag_data[i] - magmeter.hard_iron[i];
  }

  //grabs the min and max of the matrix
  for (int i = 0; i < 3; i++) {
      if (no_hard_iron[i] < magmeter.soft_min[i])
      {
        magmeter.soft_min[i] = no_hard_iron[i];
      }
      if (no_hard_iron[i] > magmeter.soft_max[i])
      {
        magmeter.soft_max[i] = no_hard_iron[i];
      }
  }


//computes the diagonal
float radius[3];
for (int i = 0; i < 3; i++)
    radius[i] = (magmeter.soft_max[i] - magmeter.soft_min[i]) / 2;
float rad_avg = (radius[0] + radius[1] + radius[2]) / 3;

// diagonal = avg_range / axis range
for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
        if (i == j)
            magmeter.soft_iron[i][j] = rad_avg / radius[i];
        else
            magmeter.soft_iron[i][j] = 0;  // no cross-axis correction
    }
}

float calibrated[3] = {0,0,0};
for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
        calibrated[i] += magmeter.soft_iron[i][j] * no_hard_iron[j];


//======================TEST OUTPUT==========================//
//======================TEST OUTPUT==========================//
// Raw readings
Serial.print("Raw:       ");
Serial.print("X: "); Serial.print(mag_data[0], 2);
Serial.print("  Y: "); Serial.print(mag_data[1], 2);
Serial.print("  Z: "); Serial.println(mag_data[2], 2);

// Hard-iron offsets
Serial.print("Bias:      ");
Serial.print(magmeter.hard_iron[0], 2); Serial.print(", ");
Serial.print(magmeter.hard_iron[1], 2); Serial.print(", ");
Serial.println(magmeter.hard_iron[2], 2);

// Calibrated readings
Serial.print("Calibrated:");
Serial.print(" X: "); Serial.print(calibrated[0], 2);
Serial.print("  Y: "); Serial.print(calibrated[1], 2);
Serial.print("  Z: "); Serial.println(calibrated[2], 2);

Serial.println(); // extra line for readability

  delay(500);
}
