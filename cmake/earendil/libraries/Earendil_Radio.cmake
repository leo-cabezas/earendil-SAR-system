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
#         * RadioHead (RH_RFM95.h)
# ---> Dependency of:
#         *

set(EARENDIL_RADIO_SOURCES              "")
set(EARENDIL_RADIO_COMPILE_DEFS         "")
set(EARENDIL_RADIO_LINK_LIBS            "")
set(EARENDIL_RADIO_HEADER_DIRS          "")

if(ENABLE_EARENDIL_RADIO)
set(EARENDIL_RADIO_SOURCES
        ./src/libraries/Earendil_Radio/Earendil_Radio.cpp
)
set(EARENDIL_RADIO_COMPILE_DEFS
        # None.
) 
set(EARENDIL_RADIO_LINK_LIBS    # Intended for pico-sdk / FreeRTOS library dependencies.
        # None.
)
set(EARENDIL_RADIO_HEADER_DIRS
        ./src/libraries/Earendil_Radio
)

# Enable dependencies
set(ENABLE_RADIOHEAD)

endif()

# ---------------------------------------------------------------------------------------------------------------
