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

add_library(lean_arduino_pico STATIC
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
target_compile_definitions(lean_arduino_pico PRIVATE
        # None.
)
target_link_libraries(lean_arduino_pico PRIVATE # !!!!!!!!!!!!!!!!!!! TEMPORARY !!!!!!!!!!!!!!!!!!!!!!
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
        FreeRTOS-Kernel
)
target_link_libraries(lean_arduino_pico PRIVATE  # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        lean_ArduinoCore_API
        lean_Earendil_ArduinoCompat
)
target_include_directories(lean_arduino_pico PRIVATE
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
