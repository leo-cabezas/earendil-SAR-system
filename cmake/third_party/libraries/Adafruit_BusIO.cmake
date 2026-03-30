####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- COMMON LIBRARIES:
# ---> Name: Adafruit_BusIO
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_BusIO
#         * Version: 
#         * License: 
#         * State:      Lean, Unmodified
#         * Used in:    Handheld, Node
# ---> Dependencies (libs):
#         * arduino_pico
# ---> Dependency of (libs):
#         * ???

include_guard()

add_library(Adafruit_BusIO STATIC
        ./third_party/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.cpp              # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_GenericDevice.cpp               # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_I2CDevice.cpp                   # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_SPIDevice.cpp                   # Unmodified
)
target_compile_definitions(Adafruit_BusIO PUBLIC
        # None.
)
target_link_libraries(Adafruit_BusIO PUBLIC     # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_BusIO PUBLIC     # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico       
)
target_include_directories(Adafruit_BusIO PUBLIC
        ./third_party/libraries/Adafruit_BusIO
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.h              # Unmodified
        # ./third_party/libraries/Adafruit_BusIO/Adafruit_GenericDevice.h               # Unmodified
        # ./third_party/libraries/Adafruit_BusIO/Adafruit_I2CDevice.h                   # Unmodified
        # ./third_party/libraries/Adafruit_BusIO/Adafruit_SPIDevice.h                   # Unmodified
)

# ---------------------------------------------------------------------------------------------------------------
