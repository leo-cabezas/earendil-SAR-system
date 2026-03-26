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

set(EARENDIL_USDREADER_SOURCES          "")
set(EARENDIL_USDREADER_COMPILE_DEFS     "")
set(EARENDIL_USDREADER_LINK_LIBS        "")
set(EARENDIL_USDREADER_HEADER_DIRS      "")

if (EARENDIL_USDREADER)
set(EARENDIL_USDREADER_SOURCES
        ./src/libraries/Earendil_uSDReader/Earendil_uSDReader.cpp
)
set(EARENDIL_USDREADER_COMPILE_DEFS
        EARENDIL_USREADER_ENABLED       # Compile definition to enable Earendil_Radio within the source code.
)
set(EARENDIL_USDREADER_LINK_LIBS        # ATTENTION: Intended ONLY for pico-sdk or FreeRTOS-Kernel DIRECT dependencies.
        # Don't know yet.
)
set(EARENDIL_USDREADER_HEADER_DIRS
        ./src/libraries/Earendil_uSDReader
        # Comprehensive header list for documentation purposes:
        # ./src/libraries/Earendil_uSDReader.h
)

# Enable all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
set(ENABLE_SDFAT ON)                    # Enables SdFat.

endif()
# ---------------------------------------------------------------------------------------------------------------
