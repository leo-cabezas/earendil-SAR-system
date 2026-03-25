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

#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>

#define TFT_CS       25
#define TFT_DC       24
#define TFT_RST      12
#define TFT_MOSI     3 
#define TFT_MISO     0
#define TFT_SCLK     2

#define X_OFFSET 120
#define Y_OFFSET 120

void vDisplay(void* pvParameters);
