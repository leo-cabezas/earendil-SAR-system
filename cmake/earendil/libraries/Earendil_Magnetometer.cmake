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
#         *
# ---> Dependency of:
#         *

set(EARENDIL_MAGNETOMETER_SOURCES       "")
set(EARENDIL_MAGNETOMETER_COMPILE_DEFS  "")
set(EARENDIL_MAGNETOMETER_LINK_LIBS     "")
set(EARENDIL_MAGNETOMETER_HEADER_DIRS   "")

if (ENABLE_EARENDIL_MAGNETOMETER)
set(EARENDIL_MAGNETOMETER_SOURCES
        ./src/libraries/Earendil_Magnetometer/Earendil_Magnetometer.cpp
)
set(EARENDIL_MAGNETOMETER_COMPILE_DEFS
        # None.
) 
set(EARENDIL_MAGNETOMETER_LINK_LIBS

)
set(EARENDIL_MAGNETOMETER_HEADER_DIRS
        ./src/libraries/Earendil_Magnetometer/
)
endif()

# ---------------------------------------------------------------------------------------------------------------
