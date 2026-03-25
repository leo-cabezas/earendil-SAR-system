####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // GNSS (GPS) LIBRARIES:
# ---> Name: Earendil_GPS
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         *
# ---> Dependency of:
#         *

set(EARENDIL_GPS_SOURCES        "")
set(EARENDIL_GPS_COMPILE_DEFS   "")
set(EARENDIL_GPS_LINK_LIBS      "")
set(EARENDIL_GPS_HEADER_DIRS    "")

if (ENABLE_EARENDIL_GPS)
set(EARENDIL_GPS_SOURCES
        ./src/libraries/Earendil_GPS/Earendil_GPS.cpp
)
set(EARENDIL_GPS_COMPILE_DEFS
        # None.
) 
set(EARENDIL_GPS_LINK_LIBS

)
set(EARENDIL_GPS_HEADER_DIRS
        ./src/libraries/Earendil_GPS
)
endif()

# ---------------------------------------------------------------------------------------------------------------
