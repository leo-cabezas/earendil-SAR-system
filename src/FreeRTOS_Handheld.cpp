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

#include <Earendil_Radio.h>
#include <Earendil_GPS.h>
#include <Earendil_Display.h>
#include <Earendil_Altimeter.h>
#include <Earendil_AccelGyro.h>
// #include <Earendil_uSDReader.h>
#include <Earendil_Magnetometer.h>

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

SemaphoreHandle_t g_printMutex;                                                                                                 

int main() {
    stdio_init_all();

    // Initialize printf() semaphore.
    g_printMutex = xSemaphoreCreateMutex();
    if (g_printMutex == NULL){
        printf("Failed to create mutex!\n");
        for (;;);
    }

    // Declare tasks
    TaskHandle_t 
        taskRadioTX, 
        taskGPS, 
        taskDisplay, 
        taskAltimeter, 
        taskAccelGyro, 
        taskMicroSD, 
        taskCompass
    ;

    // xTaskCreate(vRadioTX, "TaskRadioTX", 512, (void*)g_printMutex, 2, &taskRadioTX);
    // xTaskCreate(vGPS, "TaskGPS", 512, (void*)g_printMutex, 1, &taskGPS);
    xTaskCreate(vDisplay, "TaskDisplay", 512, NULL, 1, &taskDisplay);
    // xTaskCreate(vAltimeter, "TaskAltimeter", 512, NULL, 1, &taskAltimeter);
    // xTaskCreate(vAccelGyro, "TaskAccelGyro", 512, NULL, 1, &taskAccelGyro);
    // xTaskCreate(vMicroSD, "TaskMicroSD", 512, NULL, 1, &taskMicroSD);
    // xTaskCreate(vCompass, "TaskCompass", 512, NULL, 1, &taskCompass);
    
    // Pin tasks to cores
    // vTaskCoreAffinitySet(taskRadioTX, 1 << 0);     // Core 0
    // vTaskCoreAffinitySet(taskGPS, 1 << 0);         // etc.
    vTaskCoreAffinitySet(taskDisplay, 1 << 0);      
    // TaskCoreAffinitySet(taskAltimeter, 1 << 0);
    // vTaskCoreAffinitySet(taskAccelGyro, 1 << 0);
    // vTaskCoreAffinitySet(taskMicroSD, 1 << 0);
    // vTaskCoreAffinitySet(taskCompass, 1 << 0);
    
    
    vTaskStartScheduler();

    while(1); // Must not return from main() in FreeRTOS.
}
