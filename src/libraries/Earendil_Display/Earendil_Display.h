#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment
// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_GC9A01A.h>   // Adafruit_GC9A01A library

// --- ROUND LCD DISPLAY PINOUT CONFIGURATION ---
#define TFT_CS       25     // SPI chip select
#define TFT_DC       24     // DATA/COMMAND pin
#define TFT_RST      12     // RESET pin
#define TFT_MOSI     3      // SPI MOSI (Master-Out, Slave-In)
#define TFT_MISO     0      // SPI MISO (Master-In, Slave-Out)
#define TFT_SCLK     2      // SPI clock

#define X_OFFSET 60
#define Y_OFFSET 60

void vDisplay(void* pvParameters);