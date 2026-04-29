#pragma once

#include <Earendil_AccelGyro_TaskHandles.h>
#include <Earendil_Altimeter_TaskHandles.h>
#include <Earendil_Display_TaskHandles.h>
#include <Earendil_GPS_TaskHandles.h>
#include <Earendil_Magnetometer_TaskHandles.h>
#include <Earendil_Radio_TaskHandles.h>
#include <Earendil_uSDReader_TaskHandles.h>

namespace Earendil {
    typedef struct Earendil_TaskHandles {
        Earendil_AccelGyro::AccelGyro_TaskHandles_t         AccelGyro_Handles;
        Earendil_Altimeter::Altimeter_TaskHandles_t         Altimeter_Handles;
        Earendil_Display::Display_TaskHandles_t             Display_Handles;
        Earendil_GPS::GPS_TaskHandles_t                     GPS_Handles;
        Earendil_Magnetometer::Magnetometer_TaskHandles_t   Magnetometer_Handles;
        Earendil_Radio::Radio_TaskHandles_t                 Radio_Handles;
        Earendil_uSDReader::uSDReader_TaskHandles_t         uSDReader_Handles;
    } Earendil_TaskHandles_t;
}

