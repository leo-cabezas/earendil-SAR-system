#include <Earendil_Utils.h>

static inline float hypsometricAltitude(float temp, float pressure){
    float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
    return altitude;
}

// static inline float hypsometric_pressure(float temp, float pressure){
//   float sealevelpressure = (pressure) / (pow(1 - (LAPSE_RATE * LOCAL_ALTITUDE / temp), HYPSOMETRIC_CONSTANT));
//   return sealevelpressure;
// }

void vAltitudeUtility(void* pvParameters){
    //QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    SensorData_t bench;
    float temperature = 0;
    float pressure = 0;
    float altitude = 0;
    while(1){
        if (xQueueReceive(uQueue, &bench, portMAX_DELAY) == pdPASS) {
            if (bench.eSource == altTemp){
                temperature = bench.eData;
                //printf("Received temperature: %f *C\n", temperature);
            }
            if (bench.eSource == altPress){
                pressure = bench.eData / 100;
                //printf("Received pressure: %f hPa\n", pressure);
            }
            altitude = hypsometricAltitude(temperature + 273.15, pressure);
        }
        printf("Temperature: %f *C\nPressure: %f hPa\nCalculated Altitude: %f m\n", temperature, pressure, altitude);
    }
}

//void vGetPressure(void* pvParameters);