####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ACCELEROMETER + GYROSCOPE LIBRARIES: 
# ---> Name: Adafruit_LSM6DS
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_LSM6DS
#         * Version: 
#         * License: 
#         * State:      Lean, Unmodified
#         * Used in:    Handheld
# ---> Dependencies (libs):
#         * arduino-pico
#         * Adafruit_BusIO
#         * Adafruit_Sensor
# ---> Dependency of (libs):
#         * Earendil_AccelGyro

include_guard()

add_library(Adafruit_LSM6DS STATIC
        ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DS.cpp                     # Unmodified
        ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DSOX.cpp                   # Unmodified
)
target_compile_definitions(Adafruit_LSM6DS PUBLIC
        # None.
)
target_link_libraries(Adafruit_LSM6DS PUBLIC    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_LSM6DS PUBLIC    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
        Adafruit_BusIO
        Adafruit_Sensor
)
target_include_directories(Adafruit_LSM6DS PUBLIC
        ./third_party/libraries/Adafruit_LSM6DS
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DS.h                     # Unmodified
        # ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DSOX.h                   # Unmodified
)

# ---------------------------------------------------------------------------------------------------------------
