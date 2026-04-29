#pragma once    // To prevent duplicate library imports.

// ///////////////////////////////////////////////////////////////////////////////////////
// =======================================================================================
// EARENDIL_DISPLAY DEPENDENCIES
// =======================================================================================
// ///////////////////////////////////////////////////////////////////////////////////////

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
#include <queue.h>              // For IPC communication between the LCD and buttons.
#include <event_groups.h>

// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
#include <pico/multicore.h>     // Enable as needed. Not needed at the moment

// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_GC9A01A.h>   // Adafruit_GC9A01A library

// --- DEPENDENCIES // EARENDIL LIBRARIES ---
#include <Earendil_TaskHandles.h>
#include <Earendil_SharedData.h>
#include <Earendil_Mutexes.h>

// --- OTHER DEPENDENCIES ---
#include <string>

// ///////////////////////////////////////////////////////////////////////////////////////
// =======================================================================================
// EARENDIL_DISPLAY DEFINITIONS
// =======================================================================================
// ///////////////////////////////////////////////////////////////////////////////////////

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

// =======================================================================================
// NAVIGATION_UI Definitions
// =======================================================================================

#define NAV_BACKGROUND_COLOR        GC9A01A_BLACK
#define NAV_BACKGROUND_COLOR_INV    GC9A01A_WHITE

#define HEADING_TRACK_RADIUS        110.0
#define HEADING_TRIANGLE_HEIGHT     8.0
#define HEADING_NORTH_COLOR         GC9A01A_RED
#define HEADING_NORTH_COLOR_INV     GC9A01A_RED

#define BEARING_TRACK_RADIUS        HEADING_TRACK_RADIUS
#define BEARING_CIRCLE_RADIUS       5.0
#define BEARING_NODE_COLOR          GC9A01A_YELLOW
#define BEARING_NODE_COLOR_INV      GC9A01A_BLUE

#define GUIDE_RAIL_PADDING_OUTER    4.0
#define GUIDE_RAIL_PADDING_INNER    3.0
#define GUIDE_RAIL_RADIUS_OUTER     (HEADING_TRACK_RADIUS + (BEARING_CIRCLE_RADIUS + GUIDE_RAIL_PADDING_OUTER))
#define GUIDE_RAIL_RADIUS_INNER     (HEADING_TRACK_RADIUS - (BEARING_CIRCLE_RADIUS + GUIDE_RAIL_PADDING_INNER))
#define GUIDE_RAIL_COLOR            GC9A01A_WHITE
#define GUIDE_RAIL_COLOR_INV        GC9A01A_BLACK

#define CARDINAL_DIRS_PADDING       21
#define CARDINAL_DIRS_RADIUS        (HEADING_TRACK_RADIUS - CARDINAL_DIRS_PADDING)

#define CARDINAL_DIRS_TEXTSIZE_N    2
#define CARDINAL_DIRS_COLOR_N       GC9A01A_RED
#define CARDINAL_DIRS_COLOR_N_INV   

#define CARDINAL_DIRS_TEXTSIZE_W    2
#define CARDINAL_DIRS_COLOR_W       GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_W_INV   

#define CARDINAL_DIRS_TEXTSIZE_S    2
#define CARDINAL_DIRS_COLOR_S       GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_S_INV   

#define CARDINAL_DIRS_TEXTSIZE_E    2
#define CARDINAL_DIRS_COLOR_E       GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_E_INV   

#define CARDINAL_DIRS_TEXTSIZE_NW   1
#define CARDINAL_DIRS_COLOR_NW      GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_NW_INV

#define CARDINAL_DIRS_TEXTSIZE_SW   1
#define CARDINAL_DIRS_COLOR_SW      GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_SW_INV

#define CARDINAL_DIRS_TEXTSIZE_SE   1
#define CARDINAL_DIRS_COLOR_SE      GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_SE_INV

#define CARDINAL_DIRS_TEXTSIZE_NE   1
#define CARDINAL_DIRS_COLOR_NE      GC9A01A_WHITE
#define CARDINAL_DIRS_COLOR_NE_INV

#define DISTANCE_MAX_DISPL_VALUE    10000   // In meters.
#define DISTANCE_TEXT_SIZE          3
#define DISTANCE_TEXT_COLOR         0x8E7D  // Baby blue.

// =======================================================================================
// MENU_UI Definitions
// =======================================================================================

#define MENU_BACKGROUND_COLOR       GC9A01A_BLACK
#define MENU_BACKGROUND_COLOR_INV   GC9A01A_WHITE

#define X_MENU_OFFSET       40
#define Y_MENU_OFFSET       60

namespace Earendil_Display {
    // ///////////////////////////////////////////////////////////////////////////////////////
    // =======================================================================================
    // Earendil_Display_Tasks.cpp
    // =======================================================================================
    // ///////////////////////////////////////////////////////////////////////////////////////

    extern Earendil::Earendil_TaskHandles_t*    Earendil_Handles;
    extern Earendil::Earendil_SharedData_t*     Earendil_Data;
    extern Earendil::Earendil_Mutexes_t*        Earendil_Mutexes;
    
    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data,
        Earendil::Earendil_Mutexes_t*       global_Earendil_Mutexes
    );

    void createTasks(void);

    void createTask_vDisplay_UI(void);
    void vDisplay_UI(void* pvParameters);

    void createTask_vDisplay_Controls(void);
    void vDisplay_Controls(void* pvParameters);

    //void createTask_vDisplay_Calibration(void);
    //void vDisplay_Calibration(void* pvParameters);

    // ///////////////////////////////////////////////////////////////////////////////////////
    // =======================================================================================
    // Earendil_Display_Utils.cpp
    // =======================================================================================
    // ///////////////////////////////////////////////////////////////////////////////////////
    
    extern Adafruit_GC9A01A display;

    typedef enum {
        NULL_UI,
        NAVIGATION_UI,
        MENU_UI,
        TESTING_UI,
        CALIBRATING_UI
    } Display_UI_t;

    extern volatile Display_UI_t previous_ui;
    extern volatile Display_UI_t active_ui;

    void setup(void);
    void setupDisplay(void);

    // =======================================================================================
    // MENU_UI Utilities
    // =======================================================================================

    void setupMenu(void);
    void setupMenuButtons(void);
    void setupButton(
        const uint8_t BUTTON_PIN
    );
    void interruptHandler_BUTTON(
        const uint8_t BUTTON_PIN
    );
    void IO_IRQ_BANK0_Handler(void);

    void draw_MenuScreen_Static(void);
    void draw_MenuScreen_Dynamic(void);

    void menuControl(void);
    bool goBack(void);
    void selectItem(void);
    void moveDown(void);
    void moveUp(void);

    // =======================================================================================
    // NAVIGATION_UI UTILITIES
    // =======================================================================================

    typedef struct NavScreenState {
        int16_t heading_N_vertex_X0             = -1;
        int16_t heading_N_vertex_Y0             = -1;
        int16_t heading_N_vertex_X1             = -1;
        int16_t heading_N_vertex_Y1             = -1;
        int16_t heading_N_vertex_X2             = -1;
        int16_t heading_N_vertex_Y2             = -1;

        int16_t last_bearing_node_X             = -1;
        int16_t last_bearing_node_Y             = -1;

        int16_t last_cardinal_N_label_X         = -1;
        int16_t last_cardinal_N_label_Y         = -1;
        uint16_t last_cardinal_N_label_width    = -1;
        uint16_t last_cardinal_N_label_height   = -1;

        int16_t last_cardinal_W_label_X         = -1;
        int16_t last_cardinal_W_label_Y         = -1;
        uint16_t last_cardinal_W_label_width    = -1;
        uint16_t last_cardinal_W_label_height   = -1;
        int16_t last_cardinal_W_notch_X0        = -1;
        int16_t last_cardinal_W_notch_Y0        = -1;
        int16_t last_cardinal_W_notch_X1        = -1;
        int16_t last_cardinal_W_notch_Y1        = -1;

        int16_t last_cardinal_S_label_X         = -1;
        int16_t last_cardinal_S_label_Y         = -1;
        uint16_t last_cardinal_S_label_width    = -1;
        uint16_t last_cardinal_S_label_height   = -1;
        int16_t last_cardinal_S_notch_X0        = -1;
        int16_t last_cardinal_S_notch_Y0        = -1;
        int16_t last_cardinal_S_notch_X1        = -1;
        int16_t last_cardinal_S_notch_Y1        = -1;

        int16_t last_cardinal_E_label_X         = -1;
        int16_t last_cardinal_E_label_Y         = -1;
        uint16_t last_cardinal_E_label_width    = -1;
        uint16_t last_cardinal_E_label_height   = -1;
        int16_t last_cardinal_E_notch_X0        = -1;
        int16_t last_cardinal_E_notch_Y0        = -1;
        int16_t last_cardinal_E_notch_X1        = -1;
        int16_t last_cardinal_E_notch_Y1        = -1;

        int16_t last_cardinal_NW_label_X        = -1;
        int16_t last_cardinal_NW_label_Y        = -1;
        uint16_t last_cardinal_NW_label_width   = -1;
        uint16_t last_cardinal_NW_label_height  = -1;
        int16_t last_cardinal_NW_notch_X0       = -1;
        int16_t last_cardinal_NW_notch_Y0       = -1;
        int16_t last_cardinal_NW_notch_X1       = -1;
        int16_t last_cardinal_NW_notch_Y1       = -1;

        int16_t last_cardinal_SW_label_X        = -1;
        int16_t last_cardinal_SW_label_Y        = -1;
        uint16_t last_cardinal_SW_label_width   = -1;
        uint16_t last_cardinal_SW_label_height  = -1;
        int16_t last_cardinal_SW_notch_X0       = -1;
        int16_t last_cardinal_SW_notch_Y0       = -1;
        int16_t last_cardinal_SW_notch_X1       = -1;
        int16_t last_cardinal_SW_notch_Y1       = -1;

        int16_t last_cardinal_SE_label_X        = -1;
        int16_t last_cardinal_SE_label_Y        = -1;
        uint16_t last_cardinal_SE_label_width   = -1;
        uint16_t last_cardinal_SE_label_height  = -1;
        int16_t last_cardinal_SE_notch_X0       = -1;
        int16_t last_cardinal_SE_notch_Y0       = -1;
        int16_t last_cardinal_SE_notch_X1       = -1;
        int16_t last_cardinal_SE_notch_Y1       = -1;

        int16_t last_cardinal_NE_label_X        = -1;
        int16_t last_cardinal_NE_label_Y        = -1;
        uint16_t last_cardinal_NE_label_width   = -1;
        uint16_t last_cardinal_NE_label_height  = -1;
        int16_t last_cardinal_NE_notch_X0       = -1;
        int16_t last_cardinal_NE_notch_Y0       = -1;
        int16_t last_cardinal_NE_notch_X1       = -1;
        int16_t last_cardinal_NE_notch_Y1       = -1;

        int16_t last_distance_label_X           = -1;
        int16_t last_distance_label_Y           = -1;
        uint16_t last_distance_label_width      = -1;
        uint16_t last_distance_label_height     = -1;
    } NavScreenState_t;

    extern NavScreenState_t NavState;
    
    // --- STATIC UI ELEMENTS ----------------------------------------------------------------
    void draw_NavScreen_Static(void);
    void draw_GuideRails(void);

    // --- DYNAMIC UI ELEMENTS ---------------------------------------------------------------
    void draw_NavScreen_Dynamic(void);
    void draw_HeadingToNorth(
        double heading_north_deg
    );
    void clear_LastHeadingToNorth(void);
    void draw_BearingToNode(
        double heading_north_deg,
        double bearing_node_deg
    );
    void clear_LastBearingToNode(void);
    void draw_CardinalDirections(
        double heading_north_deg
    );
    void draw_CardinalLabel(
        double      cardinal_angle,
        const char* cardinal_label_text,
        uint8_t     cardinal_text_size,
        uint16_t    cardinal_color,
        int16_t&    last_cardinal_label_X,
        int16_t&    last_cardinal_label_Y,
        uint16_t&   last_cardinal_label_width,
        uint16_t&   last_cardinal_label_height
    );
    void draw_CardinalNotch(
        double      cardinal_angle,
        uint16_t    cardinal_color,
        int16_t&    last_cardinal_notch_X0,
        int16_t&    last_cardinal_notch_Y0,
        int16_t&    last_cardinal_notch_X1,
        int16_t&    last_cardinal_notch_Y1
    );
    void clear_LastCardinalDirections(void);
    void draw_DistanceToNode(
        double distance_node_m
    );
    void clear_DistanceToNode(void);

    // --- UX CONTROLS -----------------------------------------------------------------------
    void controlNav(void);
    
    // =======================================================================================
    // TESTING UTILITIES
    // =======================================================================================

    // Testing stuff.
    void drawTesting(void);
    void getBearingToNode(
        double& bearing_to_node_deg,
        double  handheld_latitude_rad,
        double  handheld_longitude_rad,
        double  node_latitude_rad,
        double  node_longitude_rad
    );
    void getDistanceToNode(
        double& distance_node_m,
        double  handheld_latitude_rad,
        double  handheld_longitude_rad,
        double  node_latitude_rad,
        double  node_longitude_rad
    );
}

/*
#define OUTLINE_BLU         0x2CB6
#define OUTLINE_DARKBLU     0x1188

// --- CALIBRATION STUFF ---
#define GYRO_EVT_CALIBRATE_REQUEST   ( 1 << 0 )
#define GYRO_EVT_CALIBRATE_COMPLETE  ( 1 << 1 )
#define GYRO_EVT_CALIBRATE_CANCELLED ( 1 << 2 )

extern QueueHandle_t calQueue;
extern EventGroupHandle_t gyroEventGroup;
*/

