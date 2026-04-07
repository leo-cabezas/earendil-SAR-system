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

void my_gpio_irq_handler(void)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // Read which GPIO(s) triggered interrupt
    uint32_t status = gpio_get_irq_event_mask(0);

    // Check if our pin caused it
    if (status & (1u << BUTTON_PIN))
    {
        // Clear the interrupt for that pin
        gpio_acknowledge_irq(BUTTON_PIN, GPIO_IRQ_EDGE_FALL);

        // Notify FreeRTOS task
        vTaskNotifyGiveFromISR(myTaskHandle, &xHigherPriorityTaskWoken);
    }

    // Request context switch if needed
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

*/

RH_RF95 rf95(RFM95_CS_PIN, RFM95_IRQ_PIN);

static inline radioSetup(SemaphoreHandle_t g_printMutex){
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

    // Should also set dBm!
}

void vHandheldRadioManager(void* pvParameters){
    SemaphoreHandle_t g_printMutex = (SemaphoreHandle_t)pvParameters;

    radioSetup();

    while (1){
        vTaskResume(taskHandheldRadioSendPing_TX);
        vTaskResume(taskHandheldRadioListen_RX);
    }
}

void vHandheldRadioSendPing_TX(void* pvParameters){
    vTaskSuspend();
    
    uint32_t ping_delay = 1000; // 1 second.

    while (1){
        uint8_t buf[64];
        buf[0] = (uint8_t) 88;
        buf[1] = (uint8_t) 88;
        buf[2] = (uint8_t) 88;
        buf[3] = (uint8_t) 88;
        buf[4] = (uint8_t) 'S';
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

        vTaskDelay(pdMS_TO_TICKS(ping_delay));  // Apply ping delay.
    }
}

void vHandheldRadioTransmitData_TX(void* pvParameters){
    vTaskSuspend();

    while (1){

    }
}

void vHandheldRadioListen_RX(void* pvParameters){
    vTaskSuspend();

    while (1){
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf95.recv(buf, &len)) {
            // Process packet
            Serial.print("Got: "); Serial.println((char*)buf);
            Serial.print("RSSI: "); Serial.println(rf95.lastRssi(), DEC);

            // Reply
            uint8_t data[] = "Hello back!";
            rf95.send(data, sizeof(data));
            rf95.waitPacketSent();
        }
    }
}

// DEBOUNCER TEST
/*
TaskHandle_t interrTest = NULL;

void gpio_handler(){
    if (gpio_get_irq_event_mask(12) & GPIO_IRQ_EDGE_FALL){  // DEBOUNCE BUTTON TEST IRQ
        gpio_set_irq_enabled(12, GPIO_IRQ_EDGE_FALL, false);
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        // Notify the task that the button was pressed
        vTaskNotifyGiveFromISR(interrTest, NULL);        // Yield to the task if it has higher priority
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void vInterrTest(void* pvParameters){
    (void)pvParameters;

    interrTest = xTaskGetCurrentTaskHandle();

    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_put(13, 1);

    const uint8_t inputPin = 12;
    gpio_init(inputPin);
    gpio_set_dir(inputPin, GPIO_IN);
    gpio_pull_up(inputPin);
    gpio_set_irq_enabled(inputPin, GPIO_IRQ_EDGE_FALL, true);

    irq_add_shared_handler(IO_IRQ_BANK0, gpio_handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
    irq_set_enabled(IO_IRQ_BANK0, true);

    while (1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        // Let tTaskHandle_t radioInterrRX = NULL;he signal settle
        vTaskDelay(pdMS_TO_TICKS(20));

        // Check stable state
        if (gpio_get(12) == 0) {  // still pressed?
            gpio_xor_mask(1u << 13);
        }

        // Wait until button is released (prevents retrigger)
        while (gpio_get(12) == 0) {
            vTaskDelay(pdMS_TO_TICKS(5));
        }

        // Small release debounce (optional but nice)
        vTaskDelay(pdMS_TO_TICKS(10));

        // Re-enable interrupt
        gpio_set_irq_enabled(12, GPIO_IRQ_EDGE_FALL, true);
    }
}
*/

/*
// INTERRUPT DRIVEN TX TEST

TaskHandle_t radioInterrRX = NULL;

void gpio_handler() {
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    gpio_acknowledge_irq(RFM95_IRQ_PIN, GPIO_IRQ_EDGE_RISE);
    vTaskNotifyGiveFromISR(radioInterrRX, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void vRadioInterrRX(void* pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(5000));

    radioInterrRX = xTaskGetCurrentTaskHandle();

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

    if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
        printf("Setting frequency...\n");
        xSemaphoreGive(g_printMutex);
    }
    rf95.setFrequency(915.0);

    gpio_init(RFM95_IRQ_PIN);
    gpio_set_dir(RFM95_IRQ_PIN, GPIO_IN);
    // gpio_pull_down(RFM95_IRQ_PIN);

    irq_add_shared_handler(IO_IRQ_BANK0, gpio_handler, PICO_SHARED_IRQ_HANDLER_DEFAULT_ORDER_PRIORITY);
    irq_set_enabled(IO_IRQ_BANK0, true);

    gpio_set_irq_enabled(RFM95_IRQ_PIN, GPIO_IRQ_EDGE_RISE, true);

    gpio_init(13);
    gpio_set_dir(13, GPIO_OUT);
    gpio_put(13, 0);

    while (1) {
        if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
            printf("Waiting for an interrupt...\n");
            xSemaphoreGive(g_printMutex);
        }

        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (xSemaphoreTake(g_printMutex, pdMS_TO_TICKS(100))) {
            printf("Interrupt detected!\n");
            xSemaphoreGive(g_printMutex);
        }

        gpio_put(13, 1);

        // Task wakes → read packet
        uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t len = sizeof(buf);

        if (rf95.recv(buf, &len)) {
            // Process packet
            Serial.print("Got: "); Serial.println((char*)buf);
            Serial.print("RSSI: "); Serial.println(rf95.lastRssi(), DEC);

            // Reply
            uint8_t data[] = "Hello back!";
            rf95.send(data, sizeof(data));
            rf95.waitPacketSent();
        }

        gpio_put(13, 0);
    }
}
*/