####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ARDUINO-TO-PICO-SDK COMPATIBILITY LIBRARIES:
# ---> Name: ArduinoCore-API
# ---> Metadata:
#         * Source:     https://github.com/arduino/ArduinoCore-API
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ARDUINOCORE_API_SOURCES             "")
set(ARDUINOCORE_API_COMPILE_DEFS        "")
set(ARDUINOCORE_API_LINK_LIBS           "")
set(ARDUINOCORE_API_HEADER_DIRS         "")

if (ENABLE_ARDUINOCORE_API)
set(ARDUINOCORE_API_SOURCES
        ./third_party/compat/arduino-pico/cores/rp2040/api/Common.cpp                       # Unmodified (???)
        ./third_party/compat/arduino-pico/cores/rp2040/api/Print.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/api/Stream.cpp
        ./third_party/compat/arduino-pico/cores/rp2040/api/String.cpp
)
set(ARDUINOCORE_API_COMPILE_DEFS

)
set(ARDUINOCORE_API_LINK_LIBS

)
set(ARDUINOCORE_API_HEADER_DIRS
        ./third_party/compat/arduino-pico/cores/rp2040/api
)
endif()

# ---------------------------------------------------------------------------------------------------------------
