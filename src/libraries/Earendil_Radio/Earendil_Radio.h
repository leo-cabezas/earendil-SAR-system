#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <RH_RF95.h>            // RadioHead library.

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>

// --- RADIO TRANSCEIVER PINOUT CONFIGURATION ---
#define RFM95_CS_PIN        16      // SPI chip-select
#define RFM95_IRQ_PIN       21      // Interrupt pin
#define RFM95_RST_PIN       17      // RESET pin
// #define RFM95_SPI_BUS       spi1    // Specify SPI bus (does not work with spi0)
// #define RFM95_SPI_MOSI      15      // SPI MOSI (Master-Out, Slave-In)
// #define RFM95_SPI_MISO      8       // SPI MISO (Master-In, Slave-Out)
// #define RFM95_SPI_SCLK      14      // SPI clock

#include <Earendil_TaskHandles.h>

namespace Earendil_Radio {
    // --------------------------------- TASKS ---------------------------------
    extern Earendil::Earendil_TaskHandles_t*    Earendil_Handles;
    extern Earendil::Earendil_SharedData_t*     Earendil_Data;
    
    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data
    );
    void createTasks(void);

    void createTask_vRadio_Ping_TX(void);
    void vRadio_Ping_TX(void* pvParameters);


    // --------------------------------- UTILS ---------------------------------
    extern RH_RF95 radio;

    void setup(void);

    void setupRadio(void);

    void sendPing_TX(void);


}