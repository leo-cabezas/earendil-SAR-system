//Prototype Code for Earendil Handheld device
#include <SPI.h>
#include <Wire.h>
#include "includes/types.h"
#include "includes/bmp.h"
#include "includes/my_gps.h"
#include "includes/gyro.h"
#include "includes/lcd.h"
#include "includes/mag.h"
#include "includes/rfm.h"

//Setup Code=========================================================================================================================
void setup() {

  Serial.begin(115200);
  while (!Serial) delay(1);
  Serial.println("Earendil: Handheld Prototype");

  rfmSetup();
  lcdSetup();
  my_gpsSetup();
  bmpSetup();
  magnetoSetup();
  gyroSetup();

}

moduleData sensorData;

void loop() {
  //Wait for GPS Read and Write to occur before anything else is read.=============================================================
  gpsReading(sensorData.gpsData, GPSBUFFSIZE);
  gpsShow(sensorData.gpsData, GPSBUFFSIZE);

  //BMP390 Operations and Readings==================================================================================================
  bmpReading(sensorData.bmpMetrics);
  bmpShow(sensorData.bmpMetrics);

  //Magnetometer Operations and Readings============================================================================================
  magnetoReading(sensorData.magMetrics);
  magnetoShow(sensorData.magMetrics);

  //Acceleromter+Gyroscope Operations and Readings==================================================================================
  gyroReading(sensorData.gyroMetrics);
  gyroShow(sensorData.gyroMetrics);

  //Print to the LCD Screen=========================================================================================================
  lcdWrite(sensorData);

  delay(1000);
}
