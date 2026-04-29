#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_GPS {
    typedef struct GPS_TaskHandles {
        TaskHandle_t task_vGPS_UpdateData;
    } GPS_TaskHandles_t;
}