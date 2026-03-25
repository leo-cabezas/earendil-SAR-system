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

// RadioHead libraries
#include <RH_RF95.h>

// Adafruit Feather RP2040 RFM95 default pins for the radio module
#define RFM95_CS_PIN     16
#define RFM95_IRQ_PIN    21
#define RFM95_RST_PIN    17

#define RFM95_SPI_BUS  spi1
#define RFM95_SPI_MOSI   15
#define RFM95_SPI_MISO    8
#define RFM95_SPI_SCLK   14

void vRadioTX(void* pvParameters);
