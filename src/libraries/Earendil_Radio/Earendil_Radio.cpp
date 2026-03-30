#include <Earendil_Radio.h> // ATTENTION: Add all library dependencies to this header.

/*
static inline double getLatitude(uint8_t raw_latitude[5]){    // TEMPORARY. Used to decode radio-received
  uint8_t degrees = raw_latitude[0];
  double minutes =

  return  ((double)(buf[1]) + (double)(buf[2]) / 100 + (double)(buf[3]) / 10000) / 60.0) * ;
}

static inline double getLongitude(uint8_t raw_longitude[6]){  // TEMPORARY. Used to decode radio-received GPS info.
  // Leo: Message me if this doesn't work properly
  return ((double)(raw_longitude[0]) * 100 + (double)(raw_longitude[0]) + ((double)(buf[2]) + (double)(buf[3]) / 100 + (double)(buf[4]) / 10000) / 60.0) * (buf[5] == 'E' ? 1 : -1);
}

static inline double getAltitude(uint8_t raw_altitude[]){
  // Not implemented yet.
  return;
}
*/

RH_RF95 rf95(RFM95_CS_PIN, RFM95_IRQ_PIN);

void vRadioTX(void* pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(5000));
    printf("Testing SerialUSB connection.");

    SemaphoreHandle_t g_printMutex = (SemaphoreHandle_t)pvParameters;
    
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

void vRadioRX(void* pvParameters) {
    Handheld_Shared_t* handheldShared = (Handheld_Shared_t*)pvParameters;
    SemaphoreHandle_t* g_printMutex = handheldShared->g_printMutex;
    RadioData_t* radioData = handheldShared->radioData;

    vTaskDelay(pdMS_TO_TICKS(5000));
    
   // Reset the LoRa module
    pinMode(RFM95_RST_PIN, OUTPUT);
    digitalWrite(RFM95_RST_PIN, LOW);
    vTaskDelay(pdMS_TO_TICKS(50));
    digitalWrite(RFM95_RST_PIN, HIGH);
    vTaskDelay(pdMS_TO_TICKS(50));

    // Initialize RF95
    if (!rf95.init()) {
        if (xSemaphoreTake(*g_printMutex, pdMS_TO_TICKS(100))) {
            printf("RH_RF95 init FAILED!\n");
            xSemaphoreGive(*g_printMutex);
        }
        vTaskSuspend(NULL);
    } else {
        if (xSemaphoreTake(*g_printMutex, pdMS_TO_TICKS(100))) {
            printf("RH_RF95 init succeeded!\n");
            xSemaphoreGive(*g_printMutex);
        }
    }

    // Set frequency (adjust for your region)
    rf95.setFrequency(915.0);

    uint8_t count = 0;
    while (1) {
        if (rf95.available()) {
            // Should be a message for us now
            uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t len = sizeof(buf);

            if (rf95.recv(buf, &len)) {
                // radioData->RECV = buf;
                // radioData->lastRSSI = rf95.lastRssi();
                
                digitalWrite(LED_BUILTIN, HIGH);
                RH_RF95::printBuffer("Received: ", buf, len);
                Serial.print("Got: ");
                Serial.println((char*)buf);
                Serial.print("RSSI: ");
                Serial.println(rf95.lastRssi(), DEC);

                // Send a reply
                uint8_t data[] = "And hello back to you";
                rf95.send(data, sizeof(data));
                rf95.waitPacketSent();
                Serial.println("Sent a reply");
                digitalWrite(LED_BUILTIN, LOW);
            } else {
                Serial.println("Receive failed");
            }
        }
    }
}

