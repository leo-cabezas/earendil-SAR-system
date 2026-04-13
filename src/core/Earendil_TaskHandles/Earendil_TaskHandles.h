#pragma once

#include <Earendil_Radio_TaskHandles.h>
// GPS
#include <Earendil_Display_TaskHandles.h>
// Altimeter
// AccelGyro
// uSDReader
#include <Earendil_Magnetometer_TaskHandles.h>

namespace Earendil {
    typedef struct Earendil_TaskHandles {
        Earendil_Radio::Radio_TaskHandles_t Radio_Handles;
        // GPS
        Earendil_Display::Display_TaskHandles_t Display_Handles;
        // Altimeter
        // AccelGyro
        // uSDReader
        Earendil_Magnetometer::Magnetometer_TaskHandles_t Magnetometer_Handles;
    } Earendil_TaskHandles_t;
}

