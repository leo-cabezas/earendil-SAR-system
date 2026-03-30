#include <Earendil_Utils.h>

void vAltitudeUtility(void* pvParameters){
    QueueHandle_t xQueue = (QueueHandle_t)pvParameters;
    uint32_t temperature;
    uint32_t pressure = 900;
    while(1){
        if (xQueueReceive(xQueue, &temperature, portMAX_DELAY) == pdPASS) {
            float altitude = (temperature / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
            printf("Received altitude: %f\n", altitude);
        }
    }
}

//void vGetPressure(void* pvParameters);