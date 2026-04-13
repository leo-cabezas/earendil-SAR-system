#pragma once

// TEMPORARY !!!!!!!!!!!!!!!!!!!!!!!!!!
#include <cstdio>   // Probably not needed here. Just in case.
#include <string>

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
// pico-sdk libraries
#include <pico/stdlib.h>
#include <pico/multicore.h>

#ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
    #include <Earendil_Radio.h>
#endif
#ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
    #include <Earendil_GPS.h>
#endif
#ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
    #include <Earendil_Display.h>
#endif
#ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
    #include <Earendil_Altimeter.h>
#endif
#ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
    #include <Earendil_AccelGyro.h>
#endif
#ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
    #include <Earendil_uSDReader.h>
#endif
#ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
    #include <Earendil_Magnetometer.h>
#endif

#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>

// #include <Earendil_Utils.h>

namespace Earendil {
    void setup(void);
    void createTasks(void);
}

