#pragma once

#include <FreeRTOS.h>
#include <semphr.h>

namespace Earendil {
    typedef struct Earendil_Mutexes {
        SemaphoreHandle_t spi_mutex;
    } Earendil_Mutexes_t;
}