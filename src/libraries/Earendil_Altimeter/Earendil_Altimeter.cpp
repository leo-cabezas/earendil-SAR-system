#include <Earendil_Altimeter.h> // ATTENTION: Add all library dependencies to this header.

Adafruit_BMP3XX bmp;

static inline float hypsometric_altitude(float temp, float pressure){
  float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
  return altitude;
}

static inline float hypsometric_pressure(float temp, float pressure){
  float sealevelpressure = (pressure) / (pow(1 - (LAPSE_RATE * LOCAL_ALTITUDE / temp), HYPSOMETRIC_CONSTANT));
  return sealevelpressure;
}


void vAltimeter(void* pvParameters){
  // (void) pvParameters;
  // ModuleData_t* sensorData = (ModuleData_t*)pvParameters;
  
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
  
  while (1){
    if (! bmp.performReading()) {
      printf("Failed to perform reading :(");
      vTaskDelete(NULL);
    }
    sensorData->bmpMetrics[0] = bmp.temperature;
    sensorData->bmpMetrics[1] = bmp.pressure / 100.0;
    sensorData->bmpMetrics[2] = hypsometric_altitude((sensorData->bmpMetrics[0] + 273.15), sensorData->bmpMetrics[1]);
    sensorData->bmpMetrics[3] = hypsometric_pressure((sensorData->bmpMetrics[0] + 273.15), sensorData->bmpMetrics[1]);
    
    char t_buf[32];
    snprintf(t_buf, sizeof(t_buf), "%.2f", sensorData->bmpMetrics[0]);
    printf("Temperature = %s *C\n", t_buf);

    char p_buf[32];
    snprintf(p_buf, sizeof(p_buf), "%.2f", sensorData->bmpMetrics[1]);
    printf("Pressure = %s hPa\n", p_buf);

    //float hyp_alt = hypsometric_altitude((bmp.temperature + 273.15), bmp.pressure / 100);
    char a_buf[32];
    snprintf(a_buf, sizeof(a_buf), "%.2f", sensorData->bmpMetrics[2]);
    printf("Approx. Altitude = %s m\n", a_buf);

    char lsp_buf[32];
    snprintf(lsp_buf, sizeof(lsp_buf), "%.2f", sensorData->bmpMetrics[3]);
    printf("Approx. Sea Level Pressure = %s m\n", lsp_buf);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

void vAltimeter_test(void* pvParameters){
  ModuleData_t* sensorData = (ModuleData_t*)pvParameters;
  vTaskDelay(pdMS_TO_TICKS(7000));

  while(1){
  char t_buf[32];
  snprintf(t_buf, sizeof(t_buf), "%.2f", sensorData->bmpMetrics[0]);
  printf("Test Temperature = %s *C\n", t_buf);

  vTaskDelay(pdMS_TO_TICKS(5000));
  }
}
