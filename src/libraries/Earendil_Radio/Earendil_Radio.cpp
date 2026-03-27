#include <Earendil_Radio.h> // ATTENTION: Add all library dependencies to this header.

RH_RF95 rf95(RFM95_CS_PIN, RFM95_IRQ_PIN);

void vRadioTX(void* pvParameters) {
    SemaphoreHandle_t g_printMutex = (SemaphoreHandle_t)pvParameters;

    vTaskDelay(pdMS_TO_TICKS(5000));
    
   // Reset the LoRa module
    pinMode(RFM95_RST_PIN, OUTPUT);
    digitalWrite(RFM95_RST_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(50));
    digitalWrite(RFM95_RST_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(50));

    // Initialize RF95
    if (!rf95.init()) {
        if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
            printf("RH_RF95 init FAILED!\n");
            xSemaphoreGive(g_printMutex);
        }
        vTaskSuspend(NULL);
    } else {
        if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
            printf("RH_RF95 init succeeded!\n");
            xSemaphoreGive(g_printMutex);
        }
    }

    // Set frequency (adjust for your region)
    rf95.setFrequency(915.0);

    uint8_t count = 0;
    while (1) {
        uint8_t buf[64];
        buf[0] = (uint8_t) 40;
        buf[1] = (uint8_t) 42;
        buf[2] = (uint8_t) 61;
        buf[3] = (uint8_t) 92;
        buf[4] = (uint8_t) 'N';
        int len = 5;

        // Send packet
        if (rf95.send((uint8_t*)buf, len + 1)) {
            rf95.waitPacketSent();
            if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
                printf("TX: %s\n", (char*)buf);
                xSemaphoreGive(g_printMutex);
            }
        } else {
            if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
                printf("TX failed\n");
                xSemaphoreGive(g_printMutex);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(2000)); // send every second
    }
}
