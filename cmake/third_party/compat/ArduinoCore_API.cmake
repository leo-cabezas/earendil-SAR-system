####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ARDUINO-TO-PICO-SDK COMPATIBILITY LIBRARIES:
# ---> Name: ArduinoCore-API
# ---> Metadata:
#         * Source:     https://github.com/arduino/ArduinoCore-API
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

include_guard()

add_library(ArduinoCore_API STATIC
        ./third_party/compat/arduino-pico/cores/rp2040/api/Common.cpp           # Unmodified (???)
        ./third_party/compat/arduino-pico/cores/rp2040/api/Print.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/api/Stream.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/api/String.cpp
)
target_compile_definitions(ArduinoCore_API PUBLIC        
        # None.
)
target_link_libraries(ArduinoCore_API PUBLIC   # !!!!!!!!!!!!!!!!!!! TEMPORARY !!!!!!!!!!!!!!!!!!!!!!
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
target_link_libraries(ArduinoCore_API PUBLIC   # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        # None.
)
target_include_directories(ArduinoCore_API PUBLIC
        ./third_party/compat/arduino-pico/cores/rp2040/api
        # Comprehensive header list for documentation purposes:
        # 
)

# ---------------------------------------------------------------------------------------------------------------
