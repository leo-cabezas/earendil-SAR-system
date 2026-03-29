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

include_guard()

add_library(Earendil_Altimeter STATIC
        ./src/libraries/Earendil_Altimeter/Earendil_Altimeter.cpp
)
target_compile_definitions(Earendil_Altimeter PUBLIC
        EARENDIL_ALTIMETER_ENABLED      # Compile definition to enable Earendil_Altimeter within the source code.
) 
target_link_libraries(Earendil_Altimeter PUBLIC # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_Altimeter PUBLIC # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        Adafruit_BMP3XX
)
target_include_directories(Earendil_Altimeter PUBLIC
        ./src/libraries/Earendil_Altimeter
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Altimeter.h
)

# ---------------------------------------------------------------------------------------------------------------
