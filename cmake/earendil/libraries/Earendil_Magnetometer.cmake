####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // MAGNETOMETER LIBRARIES:
# ---> Name: Earendil_Magnetometer
# ---> Metadata:
#         * Used in:    Handheld
# ---> Dependencies:
#         * Adafruit_MMC56x3
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp

set(EARENDIL_MAGNETOMETER_SOURCES       "")
set(EARENDIL_MAGNETOMETER_COMPILE_DEFS  "")
set(EARENDIL_MAGNETOMETER_LINK_LIBS     "")
set(EARENDIL_MAGNETOMETER_HEADER_DIRS   "")

if (EARENDIL_MAGNETOMETER)
set(EARENDIL_MAGNETOMETER_SOURCES
        ./src/libraries/Earendil_Magnetometer/Earendil_Magnetometer.cpp
)
set(EARENDIL_MAGNETOMETER_COMPILE_DEFS
        EARENDIL_MAGNETOMETER_ENABLED   # Compile definition to enable Earendil_Radio within the source code.
) 
set(EARENDIL_MAGNETOMETER_LINK_LIBS     # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib                     # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        FreeRTOS-Kernel                 # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_MAGNETOMETER_HEADER_DIRS
        ./src/libraries/Earendil_Magnetometer
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Magnetometer.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_ADAFRUIT_MMC56X3 ON)         # Enables Adafruit_MMC56x3.

endif()

# ---------------------------------------------------------------------------------------------------------------
