#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Display {
    typedef struct Display_TaskHandles {
        TaskHandle_t task_vDisplay_NavScreen;
        TaskHandle_t task_vDisplay_MenuScreen;
        TaskHandle_t task_vDisplay_Calibration;
    } Display_TaskHandles_t;
}