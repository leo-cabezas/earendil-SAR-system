#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_AccelGyro {
    typedef struct AccelGyro_TaskHandles {
        TaskHandle_t task_vAccelGyro;
    } AccelGyro_TaskHandles_t;
}