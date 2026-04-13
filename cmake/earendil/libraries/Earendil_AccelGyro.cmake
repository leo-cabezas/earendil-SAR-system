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

include_guard()

add_library(Earendil_AccelGyro STATIC
        ./src/libraries/Earendil_AccelGyro/Earendil_AccelGyro.cpp
)
target_compile_definitions(Earendil_AccelGyro PUBLIC
        EARENDIL_ACCELGYRO_ENABLED      # Compile definition to enable Earendil_GPS within the source code.
) 
target_link_libraries(Earendil_AccelGyro PUBLIC # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_AccelGyro PUBLIC # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # cmake/earendil/core
        Earendil_TaskHandles
        Earendil_SharedData
        # cmake/earendil/libraries
        Adafruit_LSM6DS
)
target_include_directories(Earendil_AccelGyro PUBLIC
        ./src/libraries/Earendil_AccelGyro
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_AccelGyro.h
)

# ---------------------------------------------------------------------------------------------------------------
