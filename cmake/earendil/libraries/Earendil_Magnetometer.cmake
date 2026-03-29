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

include_guard()

add_library(Earendil_Magnetometer STATIC
        ./src/libraries/Earendil_Magnetometer/Earendil_Magnetometer.cpp
)
target_compile_definitions(Earendil_Magnetometer PUBLIC
        EARENDIL_MAGNETOMETER_ENABLED   # Compile definition to enable Earendil_Radio within the source code.
) 
target_link_libraries(Earendil_Magnetometer PUBLIC      # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_Magnetometer PUBLIC      # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        Adafruit_MMC56x3
)
target_include_directories(Earendil_Magnetometer PUBLIC
        ./src/libraries/Earendil_Magnetometer
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Magnetometer.h
)

# ---------------------------------------------------------------------------------------------------------------
