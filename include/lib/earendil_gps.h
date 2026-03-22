#pragma once

// Third-party libraries
// FreeRTOS libraries
#include <FreeRTOS.h>
#include <task.h>
#include <semphr.h>
// pico-sdk libraries
#include <pico/stdlib.h>
#include <pico/multicore.h>
// arduino-pico-lean libraries
#include <Arduino.h>

// Adafruit_GPS_Library libraries
#include <Adafruit_GPS.h>

#define GPSSerial Serial1

#define GPSECHO false

void vGPS(void* pvParameters);
