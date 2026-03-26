####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // ACCELEROMETER + GYROSCOPE LIBRARIES:
# ---> Name: Earendil_AccelGyro
# ---> Metadata:
#         * Used in:    Handheld
# ---> Dependencies:
#         * Adafruit_LSM6DS
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp

set(EARENDIL_ACCELGYRO_SOURCES          "")
set(EARENDIL_ACCELGYRO_COMPILE_DEFS     "")
set(EARENDIL_ACCELGYRO_LINK_LIBS        "")
set(EARENDIL_ACCELGYRO_HEADER_DIRS      "")

if (EARENDIL_ACCELGYRO)
set(EARENDIL_ACCELGYRO_SOURCES
        ./src/libraries/Earendil_AccelGyro/Earendil_AccelGyro.cpp
)
set(EARENDIL_ACCELGYRO_COMPILE_DEFS
        EARENDIL_ACCELGYRO_ENABLED      # Compile definition to enable Earendil_GPS within the source code.
) 
set(EARENDIL_ACCELGYRO_LINK_LIBS        # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib                     # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        FreeRTOS-Kernel                 # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_ACCELGYRO_HEADER_DIRS
        ./src/libraries/Earendil_AccelGyro
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_AccelGyro.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_ADAFRUIT_LSM6DS ON)          # Enables Adafruit_LSM6DS.

endif()

# ---------------------------------------------------------------------------------------------------------------
