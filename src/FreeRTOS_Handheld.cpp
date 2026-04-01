/*
####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################
*/

// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
// pico-sdk libraries
#include <pico/stdlib.h>
#include <pico/multicore.h>

#include <cstdio>   // Probably not needed here. Just in case.

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

#include <Earendil_Utils.h>

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

QueueHandle_t uQueue = xQueueCreate(10, sizeof(SensorData_t));
SemaphoreHandle_t g_printMutex;


int main() {
    stdio_init_all();

    // Initialize printf() semaphore.
    g_printMutex = xSemaphoreCreateMutex();
    if (g_printMutex == NULL){
        printf("Failed to create mutex!\n");
        while(1);
    }

    #ifdef EARENDIL_RADIO_ENABLED           // Defined in Earendil_Radio.cmake, when linked to CMakeLists.txt.
        /*
        TaskHandle_t taskRadioTX;
        xTaskCreate(
            vRadioTX, 
            "TaskRadioTX", 
            512, 
            (void*)g_printMutex, 
            2, 
            &taskRadioTX
        );
        vTaskCoreAffinitySet(taskRadioTX, 1 << 0);
        */
        /*
        TaskHandle_t taskRadioRX;
        xTaskCreate(
            vRadioRX, 
            "TaskRadioRX", 
            512, 
            (void*)g_printMutex, 
            2, 
            &taskRadioRX
        );
        vTaskCoreAffinitySet(taskRadioRX, 1 << 0);
        */
        /*
        TaskHandle_t interrTest;
        xTaskCreate(
            vInterrTest, 
            "interrTest", 
            512, 
            NULL, 
            2, 
            &interrTest
        );
        vTaskCoreAffinitySet(interrTest, 1 << 0);
        */
        
        TaskHandle_t taskRadioInterrRX;
        xTaskCreate(
            vRadioInterrRX, 
            "TaskRadioInterrRX", 
            512, 
            (void*)g_printMutex, 
            2, 
            &taskRadioInterrRX
        );
        vTaskCoreAffinitySet(taskRadioInterrRX, 1 << 0);
        
    #endif
    #ifdef EARENDIL_GPS_ENABLED             // Defined in Earendil_GPS.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskGPS;
        xTaskCreate(
            vGPS, 
            "TaskGPS", 
            512, 
            NULL, 
            1, 
            &taskGPS
        );
        vTaskCoreAffinitySet(taskGPS, 1 << 0);
    #endif
    #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskDisplay;
        xTaskCreate(
            vDisplay, 
            "TaskDisplay", 
            512, 
            NULL, 
            1, 
            &taskDisplay
        );
        vTaskCoreAffinitySet(taskDisplay, 1 << 0);
    #endif
    #ifdef EARENDIL_ALTIMETER_ENABLED       // Defined in Earendil_Altimeter.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskAltimeter;
        xTaskCreate(
            vAltimeter, 
            "TaskAltimeter", 
            512, 
            NULL,
            2, 
            &taskAltimeter
        );
        vTaskCoreAffinitySet(taskAltimeter, 1 << 0);
        TaskHandle_t taskAltitudeUtility;
        xTaskCreate(
            vAltitudeUtility,
            "TaskAltitudeUtility",
            512,
            NULL,
            1,
            &taskAltitudeUtility
        );
        vTaskCoreAffinitySet(taskAltitudeUtility, 1 << 0);
    #endif
    #ifdef EARENDIL_ACCELGYRO_ENABLED       // Defined in Earendil_AccelGyro.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskAccelGyro;
        xTaskCreate(
            vAccelGyro, 
            "TaskAccelGyro", 
            512, 
            NULL, 
            1, 
            &taskAccelGyro
        );
        vTaskCoreAffinitySet(taskAccelGyro, 1 << 0);
    #endif
    #ifdef EARENDIL_USDREADER_ENABLED       // Defined in Earendil_uSDReader.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskMicroSD;
        xTaskCreate(
            vMicroSD, 
            "TaskMicroSD", 
            512, 
            NULL, 
            1, 
            &taskMicroSD
        );
        vTaskCoreAffinitySet(taskMicroSD, 1 << 0);
    #endif
    #ifdef EARENDIL_MAGNETOMETER_ENABLED    // Defined in Earendil_Magnetometer.cmake, when linked to CMakeLists.txt.
        TaskHandle_t taskCompass;
        xTaskCreate(
            vCompass, 
            "TaskCompass", 
            512, 
            NULL, 
            1, 
            &taskCompass
        );
        vTaskCoreAffinitySet(taskCompass, 1 << 0);
    #endif
    
    vTaskStartScheduler();

    while(1); // Must not return from main() in FreeRTOS.
}
