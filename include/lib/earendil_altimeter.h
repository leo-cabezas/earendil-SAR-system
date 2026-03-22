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

#include <Adafruit_BMP3XX.h>

#define SEALEVELPRESSURE_HPA (1013.25)

void vAltimeter(void* pvParameters);
