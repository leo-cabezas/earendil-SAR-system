####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ARDUINO-TO-PICO-SDK COMPATIBILITY LIBRARIES:
# ---> Name: arduino-pico
# ---> Metadata:
#         * Source:     https://github.com/earlephilhower/arduino-pico
#         * Version:    
#         * License:    
#         * State:      Lean, Modified
#         * Used in:    Handheld, Node
# ---> Dependencies (libs):
#         * Earendil_ArduinoCompat (due to modification)
#         * ArduinoCore-API
# ---> Dependency of (libs):
#         * ??????????????????????
#         * RadioHead
#         * 

add_library(arduino_pico STATIC
        # cores/rp2040
        ./third_party/compat/arduino-pico/cores/rp2040/_freertos.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/Bootsel.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/CoreMutex.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/PIOProgram.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/RP2040Support.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/SerialPIO.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/SerialUART.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/stdlib_noniso.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/delay.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/wiring_analog.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/wiring_digital.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/wiring_private.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/WMath.cpp
        # libraries/SPI
        ./third_party/compat/arduino-pico/libraries/SPI/SPI.cpp
        # libraries/Wire
        ./third_party/compat/arduino-pico/libraries/Wire/Wire.cpp
)
target_compile_definitions(arduino_pico PUBLIC
        # None.
)
target_link_libraries(arduino_pico PUBLIC # !!!!!!!!!!!!!!!!!!! TEMPORARY !!!!!!!!!!!!!!!!!!!!!!
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4

        pico_stdlib        
        pico_rand

        hardware_dma
        hardware_spi
        hardware_irq
        hardware_sync
        hardware_clocks
        hardware_pio
        hardware_exception
        hardware_watchdog
        hardware_claim
        hardware_i2c
        hardware_pwm
        hardware_adc
)
target_link_libraries(arduino_pico PUBLIC  # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        ArduinoCore_API
        Earendil_ArduinoCompat
)
target_include_directories(arduino_pico PUBLIC
        ./third_party/compat/arduino-pico/cores/rp2040        
        ./third_party/compat/arduino-pico/variants/adafruit_feather_rfm
        ./third_party/compat/arduino-pico/libraries/SPI
        ./third_party/compat/arduino-pico/libraries/Wire
        # Comprehensive header list for documentation purposes:
        # cores/rp2040

        # variants/adafruit_feather_rfm

        # libraries/SPI

        # libraries/Wire

)

# ---------------------------------------------------------------------------------------------------------------
