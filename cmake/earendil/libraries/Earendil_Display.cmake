####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // ROUND LCD DISPLAY LIBRARIES:
# ---> Name: Earendil_Display
# ---> Metadata:
#         * Used in:    Handheld
# ---> Dependencies:
#         *
# ---> Dependency of:
#         * Earendil

set(EARENDIL_DISPLAY_SOURCES            "")
set(EARENDIL_DISPLAY_COMPILE_DEFS       "")
set(EARENDIL_DISPLAY_LINK_LIBS          "")
set(EARENDIL_DISPLAY_HEADER_DIRS        "")

if (ENABLE_EARENDIL_DISPLAY)
set(EARENDIL_DISPLAY_SOURCES
        ./src/libraries/Earendil_Display/Earendil_Display.cpp
)
set(EARENDIL_DISPLAY_COMPILE_DEFS
        # None.
) 
set(EARENDIL_DISPLAY_LINK_LIBS

)
set(EARENDIL_DISPLAY_HEADER_DIRS
        ./src/libraries/Earendil_Display
)
endif()

# ---------------------------------------------------------------------------------------------------------------
