####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // RADIO TRANSCEIVER LIBRARIES:
# ---> Name: Earendil_Radio
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * RadioHead
#         * pico-sdk
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp
#         * FreeRTOS_Node.cpp

include_guard()

add_library(Earendil_Radio STATIC
        ./src/libraries/Earendil_Radio/Earendil_Radio_Tasks.cpp
        ./src/libraries/Earendil_Radio/Earendil_Radio_Utils.cpp
)
target_compile_definitions(Earendil_Radio PUBLIC
        EARENDIL_RADIO_ENABLED  # Compile definition to enable Earendil_Radio within the source code.
) 
target_link_libraries(Earendil_Radio PUBLIC    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_Radio PUBLIC    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # cmake/earendil/core
        Earendil_TaskHandles
        Earendil_SharedData
        Earendil_Mutexes
        # cmake/earendil/libraries
        RadioHead
)
target_include_directories(Earendil_Radio PUBLIC
        ./src/libraries/Earendil_Radio
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Radio.h
)

# ---------------------------------------------------------------------------------------------------------------
