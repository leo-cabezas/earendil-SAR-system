#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Radio {
    typedef struct Radio_TaskHandles {
        TaskHandle_t task_vRadio_Ping_TX;
        TaskHandle_t task_vRadio_Listen_RX;
    } Radio_TaskHandles_t;
}