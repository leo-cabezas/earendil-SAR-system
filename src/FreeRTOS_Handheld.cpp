/*
####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################
*/

#include <FreeRTOS_Handheld.h>

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

QueueHandle_t auQueue = xQueueCreate(2, sizeof(SensorData_t));
QueueHandle_t guQueue = xQueueCreate(2, sizeof(GPSData_t));
QueueHandle_t calQueue = xQueueCreate(6, 64);
SemaphoreHandle_t g_printMutex;
SemaphoreHandle_t gpsDataMutex;

Earendil_Task_Handles_t Earendil_Task_Handles;

int main() {
    stdio_init_all();

    // Initialize printf() semaphore.
    g_printMutex = xSemaphoreCreateMutex();
    if (g_printMutex == NULL){
        printf("Failed to create mutex!\n");
        while(1);
    }

    // Intilialize GPS struct semaphore.
    gpsDataMutex = xSemaphoreCreateMutex();
    if (gpsDataMutex == NULL) {// Not enough heap
        printf("Failed to create GPS mutex!\n");
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
        vTaskCoreAffinitySet(taskGPS, 1 << 1);

        TaskHandle_t taskGPSTX;
        xTaskCreate(
            vGPSTX,
            "TaskGPSTX",
            512,
            NULL,
            1,
            &taskGPSTX //This is declared as extern in Earendil_GPS.h, so that vGPS can see and notify it from within GPS.cpp
        );
        vTaskCoreAffinitySet(taskGPSTX, 1 << 1);

        TaskHandle_t taskGPSRXUtility;
        xTaskCreate(
            vGPSRXUtility,
            "TaskGPSRXUtility",
            512,
            NULL,
            1,
            &taskGPSRXUtility
        );
        vTaskCoreAffinitySet(taskGPSRXUtility, 1 << 0);
    #endif
    #ifdef EARENDIL_DISPLAY_ENABLED         // Defined in Earendil_Display.cmake, when linked to CMakeLists.txt.
        
        xTaskCreate(
            vDisplayNav, 
            "TaskDisplayNav", 
            512, 
            (void*)Earendil_Task_Handles, 
            1, 
            &Earendil_Task_Handles.taskDisplayNav
        );
        vTaskCoreAffinitySet(taskDisplayNav, 1 << 0);
        
        xTaskCreate(
            vDisplayMenu, 
            "TaskDisplayMenu", 
            512, 
            (void*)Earendil_Task_Handles, 
            1, 
            &Earendil_Task_Handles.taskDisplayMenu
        );
        vTaskCoreAffinitySet(taskDisplayMenu, 1 << 0);

        xTaskCreate(
            vDisplayControl, 
            "TaskDisplayControl", 
            512, 
            (void*)Earendil_Task_Handles,
            1, 
            &Earendil_Task_Handles.taskDisplayControl
        );
        vTaskCoreAffinitySet(taskDisplayControl, 1 << 0);

        TaskHandle_t taskDisplayCalibration;
        xTaskCreate(
            vDisplayCalibration, 
            "TaskDisplayCalibration", 
            512, 
            (void*)Earendil_Task_Handles, 
            1, 
            &Earendil_Task_Handles.taskDisplayCalibration
        );
        vTaskCoreAffinitySet(taskDisplayCalibration, 1 << 0);
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
            1024, 
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
