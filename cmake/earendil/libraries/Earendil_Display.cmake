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

include_guard()

add_library(Earendil_Display STATIC
        ./src/libraries/Earendil_Display/Earendil_Display_Tasks.cpp
        ./src/libraries/Earendil_Display/Earendil_Display_Utils.cpp
)
target_compile_definitions(Earendil_Display PUBLIC
        EARENDIL_DISPLAY_ENABLED        # Compile definition to enable Earendil_GPS within the source code.
) 
target_link_libraries(Earendil_Display PUBLIC   # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_Display PUBLIC   # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # cmake/earendil/core
        Earendil_TaskHandles
        Earendil_SharedData
        Earendil_Mutexes
        # cmake/earendil/libraries
        Adafruit_GC9A01A
)
target_include_directories(Earendil_Display PUBLIC
        ./src/libraries/Earendil_Display
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Display.h
)

# ---------------------------------------------------------------------------------------------------------------
