####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- COMMON LIBRARIES:      
# ---> Name: Adafruit_Sensor
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_Sensor
#         * Version:    1.1.15
#         * License:    Apache License (???)
#         * State:      Lean, Unmodified
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * arduino_pico
# ---> Dependency of:
#         * Adafruit_BMP3XX
#         * Adafruit_MMC56x3 (???)

include_guard()

add_library(Adafruit_Sensor STATIC
        ./third_party/libraries/Adafruit_Unified_Sensor/Adafruit_Sensor.cpp             # Unmodified
)
target_compile_definitions(Adafruit_Sensor PUBLIC
        ARDUINO=20308                           # Dummy Arduino version (anything >= 100 would work).
)
target_link_libraries(Adafruit_Sensor PUBLIC    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_Sensor PUBLIC    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
)
target_include_directories(Adafruit_Sensor PUBLIC
        ./third_party/libraries/Adafruit_Unified_Sensor
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_Unified_Sensor/Adafruit_Sensor.h             # Unmodified
)

# ---------------------------------------------------------------------------------------------------------------
