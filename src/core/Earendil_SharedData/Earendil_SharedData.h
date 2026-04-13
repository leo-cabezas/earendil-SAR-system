#pragma once

#include <Earendil_AccelGyro_SharedData.h>
#include <Earendil_Altimeter_SharedData.h>
// #include <Earendil_Display_SharedData.h>
#include <Earendil_GPS_SharedData.h>
#include <Earendil_Magnetometer_SharedData.h>
#include <Earendil_Radio_SharedData.h>
#include <Earendil_uSDReader_SharedData.h>

namespace Earendil {
    typedef struct Earendil_SharedData {
        Earendil_AccelGyro::AccelGyro_SharedData_t          AccelGyro_Data;
        Earendil_Altimeter::Altimeter_SharedData_t          Altimeter_Data;
        // Earendil_Display::Display_SharedData_t              Display_Data;
        Earendil_GPS::GPS_SharedData_t                      GPS_Data;
        Earendil_Magnetometer::Magnetometer_SharedData_t    Magnetometer_Data;
        Earendil_Radio::Radio_SharedData_t                  Radio_Data;
        Earendil_uSDReader::uSDReader_SharedData_t          uSDReader_Data;
    } Earendil_SharedData_t;
}