####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // ROUND LCD DISPLAY LIBRARIES:
# ---> Name: Earendil_Display
# ---> Metadata:
#         * Used in:    Handheld
# ---> Dependencies:
#         * Adafruit_GC9A01A
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp

set(EARENDIL_DISPLAY_SOURCES            "")
set(EARENDIL_DISPLAY_COMPILE_DEFS       "")
set(EARENDIL_DISPLAY_LINK_LIBS          "")
set(EARENDIL_DISPLAY_HEADER_DIRS        "")

if (EARENDIL_DISPLAY)
set(EARENDIL_DISPLAY_SOURCES
        ./src/libraries/Earendil_Display/Earendil_Display.cpp
)
set(EARENDIL_DISPLAY_COMPILE_DEFS
        EARENDIL_DISPLAY_ENABLED        # Compile definition to enable Earendil_GPS within the source code.
) 
set(EARENDIL_DISPLAY_LINK_LIBS          # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib                     # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        FreeRTOS-Kernel                 # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_DISPLAY_HEADER_DIRS
        ./src/libraries/Earendil_Display
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Display.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_ADAFRUIT_GC9A01A ON)         # Enables Adafruit_GC9A01A.

endif()

# ---------------------------------------------------------------------------------------------------------------
