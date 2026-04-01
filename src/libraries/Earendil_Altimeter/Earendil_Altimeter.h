#pragma once    // To prevent duplicate library imports.

// --- DEPENDENCIES // FreeRTOS-Kernel ---
#include <FreeRTOS.h>           // For pdMS_TO_TICKS(), etc. Needed for task.h, semphr.h, etc.
#include <task.h>               // For vTaskDelay(), etc.
#include <semphr.h>             // For SemaphoreHandle_t, xSemaphoreTake(), xSemaphoreGive(), etc.
// --- DEPENDENCIES // pico-sdk ---
#include <pico/stdlib.h>        // For gpio_init(), gpio_set_dir(), gpio_put(), I/O with printf(), etc.
// #include <pico/multicore.h>     // Enable as needed. Not needed at the moment
// --- DEPENDENCIES // THIRD-PARTY LIBRARIES ---
#include <Adafruit_BMP3XX.h>    // Adafruit_BMP3XX library.

#include <Earendil_Types.h>

// #define SEALEVELPRESSURE_HPA (1014.0)//Lawrence, KS local sea-level pressure: 202602221550
// #define LOCAL_ALTITUDE (289.0)//m
// #define LAPSE_RATE (0.0065)//K/m
// #define HYPSOMETRIC_CONSTANT (0.190284)// GAS_CONSTANT * LAPSE_RATE / GRAVITY = (287.05 J/kg*K) * (0.0065 K/m) / (9.80665 m/s²)

void vAltimeter(void* pvParameters);
