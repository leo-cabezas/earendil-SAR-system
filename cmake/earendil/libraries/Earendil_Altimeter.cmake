####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // PRECISION ALTIMETER LIBRARIES:
# ---> Name: Earendil_Altimeter
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * Adafruit_BMP3XX
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp
#         * FreeRTOS_Node.cpp

set(EARENDIL_ALTIMETER_SOURCES          "")
set(EARENDIL_ALTIMETER_COMPILE_DEFS     "")
set(EARENDIL_ALTIMETER_LINK_LIBS        "")
set(EARENDIL_ALTIMETER_HEADER_DIRS      "")

if (EARENDIL_ALTIMETER)
set(EARENDIL_ALTIMETER_SOURCES
        ./src/libraries/Earendil_Altimeter/Earendil_Altimeter.cpp
)
set(EARENDIL_ALTIMETER_COMPILE_DEFS
        EARENDIL_ALTIMETER_ENABLED      # Compile definition to enable Earendil_Altimeter within the source code.
) 
set(EARENDIL_ALTIMETER_LINK_LIBS        # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib                     # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        FreeRTOS-Kernel                 # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_ALTIMETER_HEADER_DIRS
        ./src/libraries/Earendil_Altimeter
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Altimeter.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_ADAFRUIT_BMP3XX ON)          # Enables Adafruit_BMP3XX.

endif()

# ---------------------------------------------------------------------------------------------------------------
