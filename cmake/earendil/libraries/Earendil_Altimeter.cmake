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
#         *
# ---> Dependency of:
#         *

set(EARENDIL_ALTIMETER_SOURCES          "")
set(EARENDIL_ALTIMETER_COMPILE_DEFS     "")
set(EARENDIL_ALTIMETER_LINK_LIBS        "")
set(EARENDIL_ALTIMETER_HEADER_DIRS      "")

if (ENABLE_EARENDIL_ALTIMETER)
set(EARENDIL_ALTIMETER_SOURCES
        ./src/libraries/Earendil_Altimeter/Earendil_Altimeter.cpp
)
set(EARENDIL_ALTIMETER_COMPILE_DEFS
        # None.
) 
set(EARENDIL_ALTIMETER_LINK_LIBS

)
set(EARENDIL_ALTIMETER_HEADER_DIRS
        ./src/libraries/Earendil_Altimeter
)
endif()

# ---------------------------------------------------------------------------------------------------------------
