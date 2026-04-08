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

#define X_MENU_OFFSET 40
#define Y_MENU_OFFSET 60

# define MENU_BUTTON 13
# define BACK_BUTTON 9
#define UP_BUTTON 6
#define DOWN_BUTTON 5

#define X_OFFSET 120
#define Y_OFFSET 120

#define OUTLINE_BLU 0x2CB6
#define OUTLINE_DARKBLU 0x1188

extern QueueHandle_t calQueue;

void vDisplayMenu(void* pvParameters);
void vDisplayControl(void* pvParameters);
void vDisplayNav(void* pvParameters);