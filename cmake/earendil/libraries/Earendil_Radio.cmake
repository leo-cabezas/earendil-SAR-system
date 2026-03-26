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

set(EARENDIL_RADIO_SOURCES              "")
set(EARENDIL_RADIO_COMPILE_DEFS         "")
set(EARENDIL_RADIO_LINK_LIBS            "")
set(EARENDIL_RADIO_HEADER_DIRS          "")

if (EARENDIL_RADIO)
set(EARENDIL_RADIO_SOURCES
        ./src/libraries/Earendil_Radio/Earendil_Radio.cpp
)
set(EARENDIL_RADIO_COMPILE_DEFS
        EARENDIL_RADIO_ENABLED  # Compile definition to enable Earendil_Radio within the source code.
) 
set(EARENDIL_RADIO_LINK_LIBS    # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        pico_stdlib             # Includes hardware_divider, hardware_gpio, hardware_uart, pico_runtime, pico_platform, pico_stdio, pico_time, and pico_util.
        freertos_kernel         # Includes all relevant FreeRTOS libraries.
)
set(EARENDIL_RADIO_HEADER_DIRS
        ./src/libraries/Earendil_Radio
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_Radio.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_RADIOHEAD ON)        # Enables RadioHead.

endif()

# ---------------------------------------------------------------------------------------------------------------
