#include <Wire.h>
#include <Adafruit_MMC56x3.h>
#include <Adafruit_Sensor.h>

Adafruit_MMC5603 mag;

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

//======================TEST OUTPUT==========================//
  Serial.print("X: "); Serial.print(event.magnetic.x);
  Serial.print("  Y: "); Serial.print(event.magnetic.y);
  Serial.print("  Z: "); Serial.println(event.magnetic.z);

  delay(500);
}
