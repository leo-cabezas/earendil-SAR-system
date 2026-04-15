#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Display {
    typedef struct Display_TaskHandles {
        TaskHandle_t task_vDisplay_UI;
        TaskHandle_t task_vDisplay_Controls;
        TaskHandle_t task_vDisplay_Calibration;
    } Display_TaskHandles_t;
}