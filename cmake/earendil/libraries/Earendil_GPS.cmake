####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // GNSS (GPS) LIBRARIES:
# ---> Name: Earendil_GPS
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * Adafruit_GPS
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp
#         * FreeRTOS_Node.cpp

set(EARENDIL_GPS_SOURCES        "")
set(EARENDIL_GPS_COMPILE_DEFS   "")
set(EARENDIL_GPS_LINK_LIBS      "")
set(EARENDIL_GPS_HEADER_DIRS    "")

if (EARENDIL_GPS)
set(EARENDIL_GPS_SOURCES
        ./src/libraries/Earendil_GPS/Earendil_GPS.cpp
)
set(EARENDIL_GPS_COMPILE_DEFS
        EARENDIL_GPS_ENABLED    # Compile definition to enable Earendil_GPS within the source code.
) 
set(EARENDIL_GPS_LINK_LIBS      # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        FreeRTOS-Kernel         # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_GPS_HEADER_DIRS
        ./src/libraries/Earendil_GPS
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_GPS.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_ADAFRUIT_GPS ON)     # Enables Adafruit_GPS.

endif()

# ---------------------------------------------------------------------------------------------------------------
