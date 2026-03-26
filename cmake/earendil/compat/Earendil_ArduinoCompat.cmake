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

if (EARENDIL_ARDUINOCOMPAT)
set(EARENDIL_ARDUINOCOMPAT_SOURCES
        ./src/compat/Earendil_ArduinoCompat/Earendil_ArduinoCompat.cpp
)
set(EARENDIL_ARDUINOCOMPAT_COMPILE_DEFS
        # None.
) 
set(EARENDIL_ARDUINOCOMPAT_LINK_LIBS

)
set(EARENDIL_ARDUINOCOMPAT_HEADER_DIRS
        ./src/compat/Earendil_ArduinoCompat
)
endif()

# ---------------------------------------------------------------------------------------------------------------
