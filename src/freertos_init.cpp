/*
####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################
*/

// Third-party libraries
// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
// pico-sdk libraries
#include <pico/stdlib.h>
#include <pico/multicore.h>

// Compatibility headers

// Task headers
#include <earendil_radio.h>
#include <earendil_gps.h>
#include <earendil_display.h>
#include <earendil_altimeter.h>
#include <earendil_accelgyro.h>
// #include <earendil_usdreader.h>
#include <earendil_magnetometer.h>

// Called every RTOS tick
void vApplicationTickHook(void) {
    // Debug print (be careful: printing every tick can flood UART)
    // printf("Tick hook called\n"); 
}

// Called if a task overflows its stack
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName) {
    (void) pxTask;
    (void) pcTaskName;
    printf("Stack overflow in task: %s\n", pcTaskName);
    // Halt
    for(;;);
}

// Called if a pvPortMalloc fails
void vApplicationMallocFailedHook(void) {
    printf("Malloc failed!\n");
    // Halt
    for(;;);
}

// Declare printf() semaphore.
SemaphoreHandle_t g_printMutex;                                                                                                 

int main() {
    stdio_init_all();

    // Initialize printf() semaphore.
    g_printMutex = xSemaphoreCreateMutex();
    if (g_printMutex == NULL){
        printf("Failed to create mutex!\n");
        for (;;);
    }

    // Create tasks
    TaskHandle_t taskRadioTX, taskGPS, taskDisplay, taskAltimeter, taskAccelGyro, taskMicroSD, taskCompass;
    // xTaskCreate(vRadioTX, "TaskRadioTX", 512, (void*)g_printMutex, 2, &taskRadioTX);
    // xTaskCreate(vGPS, "TaskGPS", 512, (void*)g_printMutex, 1, &taskGPS);
    // xTaskCreate(vDisplay, "TaskDisplay", 512, NULL, 1, &taskDisplay);
    // xTaskCreate(vAltimeter, "TaskAltimeter", 512, NULL, 1, &taskAltimeter);
    // xTaskCreate(vAccelGyro, "TaskAccelGyro", 512, NULL, 1, &taskAccelGyro);
    // xTaskCreate(vMicroSD, "TaskMicroSD", 512, NULL, 1, &taskMicroSD);
    xTaskCreate(vCompass, "TaskCompass", 512, NULL, 1, &taskCompass);
    
    // Pin tasks to cores
    // vTaskCoreAffinitySet(taskRadioTX, 1 << 0); // Core 0
    // vTaskCoreAffinitySet(taskGPS, 1 << 0); // Core 0
    // vTaskCoreAffinitySet(taskDisplay, 1 << 0);
    // vTaskCoreAffinitySet(taskAltimeter, 1 << 0);
    // vTaskCoreAffinitySet(taskAccelGyro, 1 << 0);
    // vTaskCoreAffinitySet(taskMicroSD, 1 << 0);
    vTaskCoreAffinitySet(taskCompass, 1 << 0);
    
    // Start scheduler
    vTaskStartScheduler();

    while(1); // Must not return from main() in FreeRTOS.
}
