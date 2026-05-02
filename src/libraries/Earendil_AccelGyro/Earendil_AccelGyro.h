#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
// #include <event_groups.h>       // For IPC communication between tasks during calibration.
// #include <queue.h>              // For IPC communication between the LCD and buttons.

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_LSM6DSOX.h>  // Adafruit_LSM6DS library.

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>
#include <Earendil_Mutexes.h>

// --- OTHER DEPENDENCIES ---

// --- Calibration State ------------------------------------------------
#define CALIB_SAMPLES 500
#define GRAVITY 9.80665f

// --- LCD message ------------------------------------------------------
// Adapt this struct to match your existing LCD task's message format
#define LCD_MSG_LEN 64

// --- Gyro metrics -----------------------------------------------------
// [0..2] = accel X/Y/Z (m/s^2), [3..5] = gyro X/Y/Z (rad/s)


// --- Public API ------------------------------------------------------
// void gyroSetup(QueueHandle_t lcdQueue,
//                QueueHandle_t buttonQueue,
//                SemaphoreHandle_t i2cMutex);
  
// void gyroReading(GyroMetrics_t metrics);
// void gyroShow(GyroMetrics_t metrics);
#define DELAY_VACCELGYRO 150

namespace Earendil_AccelGyro {
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

    void createTask_vAccelGyro(void);
    void vAccelGyro(void* pvParameters);
    // --------------------------------- UTILS ---------------------------------
    extern Adafruit_LSM6DSOX accelgyro;
    void gyroSetup();
    void gyroShow();
    void gyroReading();
    void gyroCalibrate();
    void setup();

}


