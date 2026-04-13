#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_GPS.h>       // Adafruit_GPS library.
#include <Earendil_Utils.h>

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>

#define GPSSerial Serial1
#define GPSECHO false

/*
extern SemaphoreHandle_t gpsDataMutex;
extern GPSData_t gpsBench;
extern TaskHandle_t taskGPSTX;
*/

namespace Earendil_GPS {
    // --------------------------------- TASKS ---------------------------------
    extern Earendil::Earendil_TaskHandles_t*    Earendil_Handles;
    extern Earendil::Earendil_SharedData_t*     Earendil_Data;
    
    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data
    );
    void createTasks(void);

    // void createTask_vGPS_(void);
    // void vGPS_(void* pvParameters);

    // --------------------------------- UTILS ---------------------------------
    extern Adafruit_GC9A01A display;

    void setup(void);
    
    
    // void vGPS(void* pvParameters);

    // void vGPSTX(void* pvParameters);


}


