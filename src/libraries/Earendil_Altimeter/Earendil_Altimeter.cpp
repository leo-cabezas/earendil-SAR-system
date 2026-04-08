#include <Earendil_Altimeter.h> // ATTENTION: Add all library dependencies to this header.

Adafruit_BMP3XX bmp;

void vAltimeter(void* pvParameters){
  
  vTaskDelay(pdMS_TO_TICKS(10000));

  printf("Adafruit BMP390 test");

  if (! bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
    printf("Could not find a valid BMP3 sensor, check wiring!");
    vTaskDelete(NULL);
  }

  // Set up oversampling and filter initialization
  bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
  bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
  bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
  bmp.setOutputDataRate(BMP3_ODR_1_5_HZ);

  SensorData_t temperature;
  temperature.eSource = altTemp;
  SensorData_t pressure;
  pressure.eSource = altPress;
  
  while (1){
    if (! bmp.performReading()) {
      printf("Failed to perform reading :(");
      vTaskDelete(NULL);
    }

    temperature.eData = bmp.temperature;
    pressure.eData = bmp.pressure;

    if (xQueueSend(auQueue, &temperature, 0) != pdPASS){
      printf("Failed to send temperature.");
      vTaskDelete(NULL);
    }
    if (xQueueSend(auQueue, &pressure, 0) != pdPASS){
      printf("Failed to send pressure.");
      vTaskDelete(NULL);
    }

    vTaskDelay(pdMS_TO_TICKS(5000));    
  }
}
