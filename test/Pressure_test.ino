#include <Wire.h>
#include <Adafruit_BMP3XX.h>
#include <math.h>
//https://www.bosch-sensortec.com/media/boschsensortec/downloads/datasheets/bst-bmp390-ds002.pdf

#define SEALEVELPRESSURE_HPA (1038.7)//Lawrence, KS local sea-level pressure: 202602221550
#define LAPSE_RATE (0.0065)//K/m
#define GAS_CONSTANT (287.05)//J/kg·K
#define GRAVITY (9.80665)//m/s²

Adafruit_BMP3XX bmp;

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("BMP390 test");
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

void loop() {
  if (bmp.performReading()) {
    Serial.print("Temp: "); Serial.print(bmp.temperature); Serial.println(" C");
    Serial.print("Pressure: "); Serial.print(bmp.pressure / 100.0); Serial.println(" hPa");
    Serial.print("Approx. Altitude = "); Serial.print(hypsometric_altitude((bmp.temperature + 273.15),(bmp.pressure / 100.0))); Serial.println(" m");

  Serial.println();
  }
  delay(2000);
}

float hypsometric_altitude(float temp, float pressure){
  float altitude = (temp / LAPSE_RATE) * (pow(SEALEVELPRESSURE_HPA / pressure, GAS_CONSTANT * LAPSE_RATE / GRAVITY) - 1);
  return altitude;
}
  
