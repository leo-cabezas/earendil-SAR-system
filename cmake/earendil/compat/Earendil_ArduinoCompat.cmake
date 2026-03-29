####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // ARDUINO-TO-PICO-SDK COMPATIBILITY LIBRARIES:
# ---> Name: Earendil_ArduinoCompat
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         *
# ---> Dependency of:
#         *

include_guard()

add_library(Earendil_ArduinoCompat STATIC
        ./src/compat/Earendil_ArduinoCompat/Earendil_ArduinoCompat.cpp
)
target_compile_definitions(Earendil_ArduinoCompat PUBLIC
        EARENDIL_ARDUINOCOMPAT_ENABLED  # Compile definition to enable Earendil_ArduinoCompat within the source code.
) 
target_link_libraries(Earendil_ArduinoCompat PUBLIC    # Link all pico-sdk / FreeRTOS-Kernel dependencies
        # None.
)
target_link_libraries(Earendil_ArduinoCompat PUBLIC    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        ArduinoCore_API
)
target_include_directories(Earendil_ArduinoCompat PUBLIC
        ./src/compat/Earendil_ArduinoCompat
        # Comprehensive header list for documentation purposes:
        # ./src/compat/Earendil_ArduinoCompat.h
)

# ---------------------------------------------------------------------------------------------------------------
