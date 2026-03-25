####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY // RADIO TRANSCEIVER LIBRARIES:
# ---> Name: RadioHead
# ---> Metadata:
#         * Source:     https://www.airspayce.com/mikem/arduino/RadioHead/
#         * Version:    1.145 (latest as of 2026-03-24)
#         * License:    Open Source Licensing GPL V3
#         * State:      Lean, 
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * arduino-pico (Arduino.h, SPI.h)
# ---> Dependency of:
#         * Earendil_Radio (Earendil_Radio.h)

set(RADIOHEAD_SOURCES           "")
set(RADIOHEAD_COMPILE_DEFS      "")
set(RADIOHEAD_LINK_LIBS         "")
set(RADIOHEAD_HEADER_DIRS       "")

if (ENABLE_RADIOHEAD)
set(RADIOHEAD_SOURCES
        ./third_party/libraries/RadioHead/RH_RF95.cpp                                   # Unmodified
        ./third_party/libraries/RadioHead/RHSPIDriver.cpp                               # Unmodified
        ./third_party/libraries/RadioHead/RHGenericDriver.cpp                           # Unmodified
        ./third_party/libraries/RadioHead/RHHardwareSPI.cpp                             # Unmodified
        ./third_party/libraries/RadioHead/RHGenericSPI.cpp                              # Unmodified
)
set(RADIOHEAD_COMPILE_DEFS
        RH_PLATFORM=RH_PLATFORM_ARDUINO         # Use Arduino libraries.
        ARDUINO=20308                           # Dummy Arduino version (anything >= 100 would work).
        ARDUINO_ARCH_RP2040                     # Use the RP2040 architecture.
        # RH_ARDUINO_ARCH_RP2040                  #
)
set(RADIOHEAD_LINK_LIBS         # Intended for pico-sdk / FreeRTOS library dependencies only.
        # None.
)
set(RADIOHEAD_HEADER_DIRS
        ./third_party/libraries/RadioHead
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/RadioHead/RH_RF95.h                                   # Unmodified
        # ./third_party/libraries/RadioHead/RHSPIDriver.h                               # Unmodified
        # ./third_party/libraries/RadioHead/RHGenericDriver.h                           # Unmodified
        # ./third_party/libraries/RadioHead/RHHardwareSPI.h                             # Unmodified
        # ./third_party/libraries/RadioHead/RHGenericSPI.h                              # Unmodified
        # ./third_party/libraries/RadioHead/RadioHead.h                                 # Unmodified
)
endif()

# ---------------------------------------------------------------------------------------------------------------
