#include "../includes/bmp.h"
#include <math.h>
#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA (1012.0)//Lawrence, KS local sea-level pressure: 20260226
#define LAPSE_RATE (0.0065)// K/m
#define GAS_CONSTANT (287.05)// J/kg·K
#define GRAVITY (9.80665)// m/s²
#define _USE_MATH_DEFINES

Adafruit_BMP3XX bmp;

void bmpSetup(){ //BMP390 Setup Function
  if (!bmp.begin_I2C()) {
    Serial.println("BMP390 not found. Check wiring!");
    while (1);
  }
  Serial.println("BMP390 found!");
  //The below parameters were chosen based on the documentation for the BMP390
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
  bmp.setOutputDataRate(BMP3_ODR_1_5_HZ);
}

void bmpReading(float* bmpMetrics){
  if (bmp.performReading()) {
    bmpMetrics[0] = bmp.temperature;
    bmpMetrics[1] = bmp.pressure / 100.0;
    bmpMetrics[2] = hypsometric_altitude((bmpMetrics[0] + 273.15), bmpMetrics[1]);
  }
}

void bmpShow(float* bmpMetrics){
  Serial.print("Temp: "); Serial.print(bmpMetrics[0]); Serial.println(" C");
  Serial.print("Pressure: "); Serial.print(bmpMetrics[1]); Serial.println(" hPa");
  Serial.print("Approx. Altitude = "); Serial.print(bmpMetrics[2]); Serial.println(" m");

}

float hypsometric_altitude(float temp, float pressure){
  float altitude = (temp / LAPSE_RATE) * (pow(SEALEVELPRESSURE_HPA / pressure, GAS_CONSTANT * LAPSE_RATE / GRAVITY) - 1);
  return altitude;
}
