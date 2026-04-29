#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Magnetometer {
    typedef struct Magnetometer_TaskHandles {
        TaskHandle_t task_vMagnetometer_UpdateHeading;
        TaskHandle_t task_vMagnetometer_Calibrate;
    } Magnetometer_TaskHandles_t;
}