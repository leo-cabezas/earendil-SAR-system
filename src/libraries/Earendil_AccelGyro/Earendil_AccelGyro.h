#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
#include <event_groups.h>       // For IPC communication between tasks during calibration.
#include <queue.h>              // For IPC communication between the LCD and buttons.
// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment
// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_LSM6DSOX.h>  // Adafruit_LSM6DS library.
#include <Earendil_Display.h>
#include <Earendil_Utils.h>

// --- Calibration State ------------------------------------------------
#define CALIB_SAMPLES 500
#define GRAVITY 9.80665f

// --- Event bits -------------------------------------------------------
#define GYRO_EVT_CALIBRATE_REQUEST  ( 1 << 0 )
#define GYRO_EVT_CALIBRATE_COMPLETE ( 1 << 1 )
#define GYRO_EVT_CALIBRATE_CANCELLED ( 1 << 2 )

// --- Button IDs -------------------------------------------------------
// #define BTN_CONFIRM  0x01
// #define BTN_CANCEL   0x02

// --- LCD message ------------------------------------------------------
// Adapt this struct to match your existing LCD task's message format
#define LCD_MSG_LEN 64

// --- Gyro metrics -----------------------------------------------------
// [0..2] = accel X/Y/Z (m/s^2), [3..5] = gyro X/Y/Z (rad/s)
typedef float GyroMetrics_t[6];

// --- Shared handles (defined in gyro.cpp, extern'd for other tasks) ---


// --- Public API ------------------------------------------------------
// void gyroSetup(QueueHandle_t lcdQueue,
//                QueueHandle_t buttonQueue,
//                SemaphoreHandle_t i2cMutex);
  
// void gyroReading(GyroMetrics_t metrics);
// void gyroShow(GyroMetrics_t metrics);

void vAccelGyro(void* pvParameters); // FreeRTOS task entry point
