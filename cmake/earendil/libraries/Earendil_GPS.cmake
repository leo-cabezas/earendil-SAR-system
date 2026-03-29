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

include_guard()

add_library(Earendil_GPS STATIC
        ./src/libraries/Earendil_GPS/Earendil_GPS.cpp
)
target_compile_definitions(Earendil_GPS PUBLIC
        EARENDIL_GPS_ENABLED    # Compile definition to enable Earendil_GPS within the source code.
) 
target_link_libraries(Earendil_GPS PUBLIC       # Link all pico-sdk / FreeRTOS-Kernel dependencies
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_GPS PUBLIC       # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        Adafruit_GPS
)
target_include_directories(Earendil_GPS PUBLIC
        ./src/libraries/Earendil_GPS
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_GPS.h
)

# ---------------------------------------------------------------------------------------------------------------
