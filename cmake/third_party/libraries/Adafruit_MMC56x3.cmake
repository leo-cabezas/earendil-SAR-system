####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- MAGNETOMETER LIBRARIES: 
# ---> Name: Adafruit_MMC56x3
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_MMC56x3
#         * Version:    1.0.2
#         * License:    BSD License (???)
#         * State:      Lean, Unmodified
#         * Used in:    Handheld
# ---> Dependencies (libs):
#         * arduino_pico
#         * Adafruit_BusIO
#         * Adafruit_Sensor
# ---> Dependency of (libs):
#         * Earendil_Magnetometer

include_guard()

add_library(Adafruit_MMC56x3 STATIC
        ./third_party/libraries/Adafruit_MMC56x3/Adafruit_MMC56x3.cpp                   # Unmodified
)
target_compile_definitions(Adafruit_MMC56x3 PUBLIC
        # None.
)
target_link_libraries(Adafruit_MMC56x3 PUBLIC   # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_MMC56x3 PUBLIC   # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
        Adafruit_BusIO
        Adafruit_Sensor
)
target_include_directories(Adafruit_MMC56x3 PUBLIC
        ./third_party/libraries/Adafruit_MMC56x3
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_MMC56x3/Adafruit_MMC56x3.h                   # Unmodified
)

# ---------------------------------------------------------------------------------------------------------------
