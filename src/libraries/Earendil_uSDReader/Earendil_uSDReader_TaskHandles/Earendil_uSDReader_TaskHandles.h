#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_uSDReader {
    typedef struct uSDReader_TaskHandles {
        TaskHandle_t task_vuSDReader;
    } uSDReader_TaskHandles_t;
}