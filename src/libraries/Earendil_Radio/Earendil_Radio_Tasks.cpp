#include <Earendil_Radio.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Radio {

    Earendil::Earendil_TaskHandles_t*   Earendil_Handles    = nullptr; 
    Earendil::Earendil_SharedData_t*    Earendil_Data       = nullptr;
    Earendil::Earendil_Mutexes_t*       Earendil_Mutexes    = nullptr;

    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data,
        Earendil::Earendil_Mutexes_t*       global_Earendil_Mutexes
    ){
        Earendil_Handles    = global_Earendil_Handles;
        Earendil_Data       = global_Earendil_Data;
        Earendil_Mutexes    = global_Earendil_Mutexes;
    }
    
    void createTasks(void){
        // createTask_vRadio_Ping_TX();
        createTask_vRadio_Listen_RX();
    }

    // =======================================================================================
    // vRadio_Ping_TX
    // =======================================================================================
    
    void createTask_vRadio_Ping_TX(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Radio_Handles.task_vRadio_Ping_TX;
        xTaskCreate(
            vRadio_Ping_TX,                                 // Task function
            "vRadio_Ping_TX",                               // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            4,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vRadio_Ping_TX(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        uint16_t recipient_id   = 1;
        uint16_t sender_id      = 0;
        uint16_t message_type   = 1;
        uint16_t message_id     = 0;
        uint16_t message_att    = 0;
        uint32_t date_sent      = 20260420;
        uint32_t time_sent      = 1333;

        while (1){
            // ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // COMMENT OUT FOR CONTINUOUS PING TEST

            // recipient_id   = 1488;
            // message_type   = 1;
            // message_id     = 0;
            message_att    += 1;
            date_sent      = 20260420;
            time_sent      = 1333;

            sendPing_TX(
                recipient_id,
                sender_id,
                message_type,
                message_id,
                message_att,
                date_sent,
                time_sent
            );

            vTaskDelay(pdMS_TO_TICKS(2000)); // FOR TESTING ONLY
        }
    }

    // =======================================================================================
    // vRadio_Listen_RX
    // =======================================================================================

    void createTask_vRadio_Listen_RX(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Radio_Handles.task_vRadio_Listen_RX;
        xTaskCreate(
            vRadio_Listen_RX,                               // Task function
            "vRadio_Listen_RX",                             // Task name (for debugging)
            1024,                                           // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            3,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            1                                               // Assigned core (options: 0, 1)
        );
    }

    void vRadio_Listen_RX(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        TickType_t xLastWakeTime;
        const TickType_t xFrequency = pdMS_TO_TICKS(DELAY_VRADIO_LISTEN_RX);
        BaseType_t xWasDelayed;
        
        xLastWakeTime = xTaskGetTickCount();
        while (1){
            listen_RX();
            xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

}


/*
void vHandheldRadioManager(void* pvParameters){
    SemaphoreHandle_t g_printMutex = (SemaphoreHandle_t)pvParameters;

    radioSetup();

    vTaskResume(taskHandheldRadioSendPing_TX);
    vTaskResume(taskHandheldRadioListen_RX);

    while (1){
        
    }
}

void vHandheldRadioTransmitData_TX(void* pvParameters){
    vTaskSuspend();

    while (1){

    }
}

void vHandheldRadioListen_RX(void* pvParameters){
    vTaskSuspend();

    uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    while (1){
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
