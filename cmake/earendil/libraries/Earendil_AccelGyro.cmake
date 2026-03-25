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
#         *
# ---> Dependency of:
#         *

set(EARENDIL_ACCELGYRO_SOURCES          "")
set(EARENDIL_ACCELGYRO_COMPILE_DEFS     "")
set(EARENDIL_ACCELGYRO_LINK_LIBS        "")
set(EARENDIL_ACCELGYRO_HEADER_DIRS      "")

if (ENABLE_EARENDIL_ACCELGYRO)
set(EARENDIL_ACCELGYRO_SOURCES
        ./src/libraries/Earendil_AccelGyro/Earendil_AccelGyro.cpp
)
set(EARENDIL_ACCELGYRO_COMPILE_DEFS
        # None.
) 
set(EARENDIL_ACCELGYRO_LINK_LIBS

)
set(EARENDIL_ACCELGYRO_HEADER_DIRS
        ./src/libraries/Earendil_AccelGyro
)
endif()

# ---------------------------------------------------------------------------------------------------------------
