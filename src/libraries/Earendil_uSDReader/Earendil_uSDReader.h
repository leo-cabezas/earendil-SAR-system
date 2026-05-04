#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment
// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <SdFat.h>              // SdFat library.

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>
#include <Earendil_Mutexes.h>


#define USD_CS 25
#define DISABLE_FS_H_WARNING
#define SD_BUFF_SIZE 128

namespace Earendil_uSDReader{
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

    void createTask_vuSDReader(void);
    void vuSDReader(void* pvParameters);

    // --------------------------------- UTILS ---------------------------------
    extern SdExFat sd;
    extern ExFile file;

    void setup();
    void vuSDReader(void* pvParameters);
    void logData();
}

