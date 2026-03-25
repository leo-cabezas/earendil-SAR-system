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
#         * 
# ---> Dependency of:
#         * 

set(EARENDIL_USDREADER_SOURCES          "")
set(EARENDIL_USDREADER_COMPILE_DEFS     "")
set(EARENDIL_USDREADER_LINK_LIBS        "")
set(EARENDIL_USDREADER_HEADER_DIRS      "")

if (ENABLE_EARENDIL_USDREADER)
set(EARENDIL_USDREADER_SOURCES
        # ./src/libraries/Earendil_uSDReader/Earendil_uSDReader.cpp
)
set(EARENDIL_USDREADER_COMPILE_DEFS
        # None.
)
set(EARENDIL_USDREADER_LINK_LIBS

)
set(EARENDIL_USDREADER_HEADER_DIRS
        # ./src/libraries/Earendil_uSDReader/
)
endif()
# ---------------------------------------------------------------------------------------------------------------
