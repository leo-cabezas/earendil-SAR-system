#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_MMC56x3.h>   // Adafruit_MMC56x3 library

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>
#include <Earendil_Mutexes.h>

// --- OTHER DEPENDENCIES ---
#include <cmath>
#include <stdio.h>


#define DELAY_VMAGNETOMETER 150


namespace Earendil_Magnetometer {
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

    void createTask_vMagnetometer_UpdateHeading(void);
    void vMagnetometer_UpdateHeading(void* pvParameters);

    void createTask_vMagnetometer_Calibrate(void);
    void vMagnetometer_Calibrate(void* pvParameters);

    // --------------------------------- UTILS ---------------------------------
    extern Adafruit_MMC5603 magnetometer;

    void setup(void);
    
    void setupMagnetometer(void);
    
    double getHeading(double calibrated[3]);

    void updateFilter(double raw[3]);
    void applyCalibration(double* calibrated);
    void calibrateMagnetometer();
}


