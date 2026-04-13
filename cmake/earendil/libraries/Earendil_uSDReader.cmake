####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // MICROSD CARD READER LIBRARIES:
# ---> Name: Earendil_uSDReader
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * SdFat
#         * ????????????????????????
# ---> Dependency of:
#         * FreeRTOS_Handheld.cpp
#         * FreeRTOS_Node.cpp

include_guard()

add_library(Earendil_uSDReader STATIC
        ./src/libraries/Earendil_uSDReader/Earendil_uSDReader.cpp
)
target_compile_definitions(Earendil_uSDReader PUBLIC
        EARENDIL_USDREADER_ENABLED       # Compile definition to enable Earendil_Radio within the source code.
)
target_link_libraries(Earendil_uSDReader PUBLIC # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
)
target_link_libraries(Earendil_uSDReader PUBLIC # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # cmake/earendil/core
        Earendil_TaskHandles
        Earendil_SharedData
        # cmake/earendil/libraries
        SdFat
)
target_include_directories(Earendil_uSDReader PUBLIC
        ./src/libraries/Earendil_uSDReader
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_uSDReader.h
)

# ---------------------------------------------------------------------------------------------------------------
