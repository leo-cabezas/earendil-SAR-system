#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
#include <queue.h>              // For IPC communication between the LCD and buttons.
#include <event_groups.h>

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_GC9A01A.h>   // Adafruit_GC9A01A library

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>

// --- OTHER DEPENDENCIES ---
// #include <Earendil_Utils.h>
#include <string>

// --- ROUND LCD DISPLAY PINOUT CONFIGURATION ---
#define TFT_CS      29     // SPI chip select
#define TFT_RST     24     // RESET pin
#define TFT_DC      25     // DATA/COMMAND pin
#define TFT_MOSI    3      // SPI MOSI (Master-Out, Slave-In)
#define TFT_MISO    0      // SPI MISO (Master-In, Slave-Out)
#define TFT_SCLK    2      // SPI clock

// --- HARDWARE BUTTONS GPIO PINS ---
#define BUTTON1         12
#define BUTTON_BACK     11
#define BUTTON_SELECT   10
#define BUTTON_DOWN     9
#define BUTTON_UP       6

// --- ROUND LCD DISPLAY SOFTWARE DEFINITIONS ---
#define DISPLAY_CENTER_X    120
#define DISPLAY_CENTER_Y    120
#define X_MENU_OFFSET       40
#define Y_MENU_OFFSET       60

#define OUTLINE_BLU         0x2CB6
#define OUTLINE_DARKBLU     0x1188

/*
// --- CALIBRATION STUFF ---
#define GYRO_EVT_CALIBRATE_REQUEST   ( 1 << 0 )
#define GYRO_EVT_CALIBRATE_COMPLETE  ( 1 << 1 )
#define GYRO_EVT_CALIBRATE_CANCELLED ( 1 << 2 )

extern QueueHandle_t calQueue;
extern EventGroupHandle_t gyroEventGroup;
*/

namespace Earendil_Display {
    // --------------------------------- TASKS ---------------------------------
    extern Earendil::Earendil_TaskHandles_t*    Earendil_Handles;
    extern Earendil::Earendil_SharedData_t*     Earendil_Data;
    
    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data
    );
    void createTasks(void);

    void createTask_vDisplay_NavScreen(void);
    void vDisplay_NavScreen(void* pvParameters);

    void createTask_vDisplay_MenuScreen(void);
    void vDisplay_MenuScreen(void* pvParameters);

    void createTask_vDisplay_Calibration(void);
    void vDisplay_Calibration(void* pvParameters);

    // --------------------------------- UTILS ---------------------------------
    extern Adafruit_GC9A01A display;

    void setup(void);

    void setupDisplay(void);
    void setupMenu(void);
    void setupMenuButtons(void);
    void setupButton(const uint8_t);
    
    void interruptHandler_BUTTON_BACK(void);
    void interruptHandler_BUTTON_SELECT(void);
    void interruptHandler_BUTTON_DOWN(void);
    void interruptHandler_BUTTON_UP(void);
    void IO_IRQ_BANK0_Handler(void);

    void drawMenu(void);
    void moveUp(void);
    void moveDown(void);
    void selectItem(void);
    bool goBack(void);
    void menuControl(void);

    void getDistNAngle(void);
    void drawDistance(void);
    void drawDirection(void);
    void drawMagneticNorth(void);
    void drawNotch(void);
    void controlNav(void);
    void displayNav(void);
}



