#pragma once

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>

namespace Earendil_Altimeter {
    typedef struct Altimeter_TaskHandles {
<<<<<<< HEAD
        TaskHandle_t task_vAltimeter_Update;
=======
        TaskHandle_t task_vAltimeter;
>>>>>>> 57615aab13e960a6adf6554990b17741fa58444b
    } Altimeter_TaskHandles_t;
}