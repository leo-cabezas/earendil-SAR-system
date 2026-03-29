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

#include <Earendil_Types.h>

// --- RADIO TRANSCEIVER PINOUT CONFIGURATION ---
#define RFM95_CS_PIN     16     // SPI chip-select
#define RFM95_IRQ_PIN    21     // Interrupt pin
#define RFM95_RST_PIN    17     // RESET pin
#define RFM95_SPI_BUS  spi1     // Specify SPI bus (does not work with spi0)
#define RFM95_SPI_MOSI   15     // SPI MOSI (Master-Out, Slave-In)
#define RFM95_SPI_MISO    8     // SPI MISO (Master-In, Slave-Out)
#define RFM95_SPI_SCLK   14     // SPI clock

void vRadioTX(void* pvParameters);

void vRadioRX(void* pvParameters);
