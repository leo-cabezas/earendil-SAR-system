/*
####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################
*/

#include <FreeRTOS_Handheld.h>

// ---------------------------------------------- Required by FreeRTOS ----------------------------------------------
void vApplicationTickHook(void) {
    // Debug print (CAREFUL: Printing every tick will flood UART).
}

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void) pcTaskName;
    printf("ERROR: Stack overflow in task: %s\n", pcTaskName);
    while(1);
}

// Called if a pvPortMalloc fails
void vApplicationMallocFailedHook(void) {
    printf("ERROR: Malloc failed!\n");
    while(1);
}
// -----------------------------------------------------------------------------------------------------------------

namespace Earendil {
    
    void setup(){
        #ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
            Earendil_Radio::setup();
        #endif
        #ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
            Earendil_GPS::setup();
        #endif
        #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
            Earendil_Display::setup();
        #endif
        #ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
            Earendil_Magnetometer::setup();
        #endif    
    }

    void createTasks(){
        static Earendil_TaskHandles_t   Earendil_Handles; // MAYBE BETTER AS AN EXTERNED GLOBAL??? Something like Earendil_Radio::Earendil_Handles = Earendil::Earendil_Handles;
        static Earendil_SharedData_t    Earendil_Data;

        #ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
            Earendil_Radio::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data
            );
            Earendil_Radio::createTasks();
        #endif
        #ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
            Earendil_Display::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data
            );
            Earendil_Display::createTasks();
        #endif
        #ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
            
        #endif
        #ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
            Earendil_Magnetometer::linkSharedStructs(
                &Earendil_Handles,
                &Earendil_Data
            );
            Earendil_Magnetometer::createTasks();
        #endif
    }
    
}

int main() {
    stdio_init_all();

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