/*
####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################
*/

#include <Earendil.h>

// ---------------------------------------------- Required by FreeRTOS ----------------------------------------------
void vApplicationTickHook(void) {
    // Debug print (CAREFUL: Printing every tick will flood UART).
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void) pcTaskName;
    puts("ERROR: Stack overflow");
    while(1);
}

// Called if a pvPortMalloc fails
void vApplicationMallocFailedHook(void) {
    puts("ERROR: Malloc failed!\n");
    while(1);
}
// -----------------------------------------------------------------------------------------------------------------

namespace Earendil {
    
    void setup(){
        #ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
            Earendil_AccelGyro::setup();
        #endif
        #ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
            Earendil_Altimeter::setup();
        #endif
        #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
            Earendil_Display::setup();
        #endif
        #ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
            Earendil_GPS::setup();
        #endif
        #ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
            Earendil_Magnetometer::setup();
        #endif
        #ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
            Earendil_Radio::setup();
        #endif
        #ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
            Earendil_uSDReader::setup();
        #endif
    }

    void createTasks(){
        static Earendil_TaskHandles_t   Earendil_Handles; // MAYBE BETTER AS AN EXTERNED GLOBAL??? Something like Earendil_Radio::Earendil_Handles = Earendil::Earendil_Handles;
        static Earendil_SharedData_t    Earendil_Data;
        
        static Earendil_Mutexes_t       Earendil_Mutexes;
        Earendil_Mutexes.spi_mutex = xSemaphoreCreateMutex();

        #ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
            Earendil_AccelGyro::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_AccelGyro::createTasks();
        #endif
        #ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
            Earendil_Altimeter::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_Altimeter::createTasks();
        #endif
        #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
            Earendil_Display::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_Display::createTasks();
        #endif
        #ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
            Earendil_GPS::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_GPS::createTasks();
        #endif
        #ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
            Earendil_Magnetometer::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_Magnetometer::createTasks();
        #endif
        #ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
            Earendil_Radio::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_Radio::createTasks();
        #endif
        #ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
            Earendil_uSDReader::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data,
                &Earendil_Mutexes
            );
            Earendil_uSDReader::createTasks();
        #endif
    }
    
}

int main() {
    // stdio_init_all();

    Earendil::setup();
    Earendil::createTasks();
    
    vTaskStartScheduler();

    while(1); // Must not return from main() in FreeRTOS.
}

/*
QueueHandle_t auQueue = xQueueCreate(2, sizeof(SensorData_t));
QueueHandle_t guQueue = xQueueCreate(2, sizeof(GPSData_t));
QueueHandle_t calQueue = xQueueCreate(6, 64);
SemaphoreHandle_t g_printMutex;
SemaphoreHandle_t gpsDataMutex;
EventGroupHandle_t gyroEventGroup = xEventGroupCreate();
*/