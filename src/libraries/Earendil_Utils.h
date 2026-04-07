#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment
// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <math.h>
#include <cstdio>

// --- GLOBALS // ALTIMETER ---
#define SEALEVELPRESSURE_HPA (1014.0)//Lawrence, KS local sea-level pressure: 202602221550
#define LOCAL_ALTITUDE (289.0)//m
#define LAPSE_RATE (0.0065)//K/m
#define HYPSOMETRIC_CONSTANT (0.190284)// GAS_CONSTANT * LAPSE_RATE / GRAVITY = (287.05 J/kg*K) * (0.0065 K/m) / (9.80665 m/s²)

// --- GLOBALS // GPS ---
#define EARTH_RADIUS 6371000.0f // This is a global average with a +-0.3% (10km) error. Be mindful of that.
// #define TEST_LAT 38.971672f
// #define TEST_LONG -95.229279f

// --- GLOBALS // STRUCTS ---
/* Defines an enumerated type used to identify the source and type of the data. */
typedef enum 
{ 
    altTemp, 
    altPress,
    magHead
} DataType_t;

/* Defines an enumerated type used to identify the source of GPS data. */
typedef enum
{
    eHandheld,
    eNode
} SysType_t;

/* Defines the structure type that will be passed on the queue for sensor data.*/
typedef struct 
{ 
    uint32_t eData; 
    DataType_t eSource;
} SensorData_t;

/* Defines the structure type that will be passed on the queue for GPS data.*/
typedef struct 
{
    SysType_t systemSource;
    float latitudeDegrees;//Positive = N, Negative = S
    float longitudeDegrees;//Positive = E, Negative = W
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t seconds;
    //bool fix;// Fix validity
} GPSData_t;


// --- GLOBALS // QUEUES ---
extern QueueHandle_t auQueue; //Queue between altimeter and utils
extern QueueHandle_t guQueue; //Queue between gps and utils

void vAltitudeUtility(void* pvParameters);

//void vGetPressure(void* pvParameters);

void vGPSRXUtility(void* pvParameters);