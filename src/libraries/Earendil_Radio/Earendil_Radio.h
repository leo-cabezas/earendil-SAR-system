#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // Core FreeRTOS library.
#include <task.h>               // FreeRTOS library. For xTaskCreate(), vTaskDelay(), vTaskNotify(), etc.
#include <semphr.h>             // FreeRTOS library. For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <RH_RF95.h>            // RadioHead library.

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>
#include <Earendil_Mutexes.h>

// --- OTHER DEPENDENCIES ---
#include <cstdint>
#include <vector>

// --- RADIO TRANSCEIVER PINOUT CONFIGURATION ---
#define RFM95_CS_PIN        16      // SPI chip-select
#define RFM95_IRQ_PIN       21      // Interrupt pin
#define RFM95_RST_PIN       17      // RESET pin
// #define RFM95_SPI_BUS       spi1    // Specify SPI bus (does not work with spi0)
// #define RFM95_SPI_MOSI      15      // SPI MOSI (Master-Out, Slave-In)
// #define RFM95_SPI_MISO      8       // SPI MISO (Master-In, Slave-Out)
// #define RFM95_SPI_SCLK      14      // SPI clock

namespace Earendil_Radio {
    // --------------------------------- TASKS ---------------------------------
    extern Earendil::Earendil_TaskHandles_t*    Earendil_Handles;
    extern Earendil::Earendil_SharedData_t*     Earendil_Data;
    extern Earendil::Earendil_Mutexes_t*        Earendil_Mutexes;
    
    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data,
        Earendil::Earendil_Mutexes_t*       global_Earendil_Mutexes
    );

    void createTasks(void);

    void createTask_vRadio_Ping_TX(void);
    void vRadio_Ping_TX(void* pvParameters);

    void createTask_vRadio_Listen_RX(void);



    // --------------------------------- UTILS ---------------------------------
    extern RH_RF95 radio;

    enum class MessageType : uint16_t {
        PING_REQUEST,
        PING_REQUEST_ACK,

        DATA_REQUEST_GPS,
        DATA_REQUEST_GPS_ACK
    }

    void setup(void);

    void setupRadio(void);

    void sendPing_TX(void);

    void encodePacket(
        std::vector<uint8_t>&       radio_packet,
        uint16_t                    recipient_id
        const std::vector<uint8_t>& metadata,
        const std::vector<uint8_t>& data,
    );

    void encodeMetadata(
        std::vector<uint8_t>&   metadata,
        uint16_t                sender_id, 
        uint16_t                message_type,
        uint16_t                message_id,
        uint16_t                message_att,
        uint32_t                date_sent,
        uint32_t                time_sent
    );


}