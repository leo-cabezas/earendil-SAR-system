#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Altimeter {
    typedef struct Altimeter_TaskHandles {
        TaskHandle_t task_vAltimeter;
    } Altimeter_TaskHandles_t;
}