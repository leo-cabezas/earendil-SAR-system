#include "../includes/mag.h"
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>

Adafruit_MMC5603 mag;
Magmeter magmeter;

void magnetoSetup(){ //Magnetometer Setup Function
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

void magnetoReading(float* magMetrics){  
  sensors_event_t event;
  mag.getEvent(&event);

  magMetrics[0] = event.magnetic.x;
  magMetrics[1] = event.magnetic.y;
  magMetrics[2] = event.magnetic.z;
  magMetrics[3] = -1*atan2(event.magnetic.x,event.magnetic.y)*180.0/M_PI;
}

void magnetoShow(float* magMetrics){
  Serial.print("Raw Magnetometer Data:       ");
  Serial.print("X: "); Serial.print(magMetrics[0], 2);
  Serial.print("  Y: "); Serial.print(magMetrics[1], 2);
  Serial.print("  Z: "); Serial.println(magMetrics[2], 2);
  Serial.print("Heading: "); Serial.println(magMetrics[3],2);

}