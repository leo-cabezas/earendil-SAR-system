#pragma once

// #include <Earendil_Radio_SharedData.h>
// GPS
// Display
// Altimeter
// AccelGyro
// uSDReader
#include <Earendil_Magnetometer_SharedData.h>

namespace Earendil {
    typedef struct Earendil_SharedData {
        // Earendil_Radio::Radio_SharedData_t Radio_Data;
        // GPS
        // Display (shouldn't have an entry, probably)
        // Altimeter
        // AccelGyro
        // uSDReader
        Earendil_Magnetometer::Magnetometer_SharedData_t Magnetometer_Data;
    } Earendil_SharedData_t;
}