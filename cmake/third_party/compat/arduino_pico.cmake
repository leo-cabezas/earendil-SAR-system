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
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ARDUINO_PICO_SOURCES        "")
set(ARDUINO_PICO_COMPILE_DEFS   "")
set(ARDUINO_PICO_LINK_LIBS      "")
set(ARDUINO_PICO_HEADER_DIRS    "")

if (ENABLE_ARDUINO_PICO)
set(ARDUINO_PICO_SOURCES
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
set(ARDUINO_PICO_COMPILE_DEFS
        # None.
)
set(ARDUINO_PICO_LINK_LIBS

)
set(ARDUINO_PICO_HEADER_DIRS
        # cores/rp2040
        ./third_party/compat/arduino-pico/cores/rp2040
        # cores/rp2040/api
        
        # variants/adafruit_feather_rfm
        ./third_party/compat/arduino-pico/variants/adafruit_feather_rfm
        # libraries/SPI
        ./third_party/compat/arduino-pico/libraries/SPI
        # libraries/Wire
        ./third_party/compat/arduino-pico/libraries/Wire
)
endif()

# ---------------------------------------------------------------------------------------------------------------
