####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ROUND LCD DISPLAY LIBRARIES:
# ---> Name: Adafruit_GC9A01A
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_GC9A01A
#         * Version:    
#         * License:    
#         * State:      
#         * Used in:    Handheld
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_GC9A01A_SOURCES            "")
set(ADAFRUIT_GC9A01A_COMPILE_DEFS       "")
set(ADAFRUIT_GC9A01A_LINK_LIBS          "")
set(ADAFRUIT_GC9A01A_HEADER_DIRS        "")

if(ENABLE_ADAFRUIT_GC9A01A)
set(ADAFRUIT_GC9A01A_SOURCES
        ./third_party/libraries/Adafruit_GC9A01A/Adafruit_GC9A01A.cpp                       # Unmodified
)
set(ADAFRUIT_GC9A01A_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_GC9A01A_LINK_LIBS

)
set(ADAFRUIT_GC9A01A_HEADER_DIRS
        ./third_party/libraries/Adafruit_GC9A01A
)
endif()

# ---------------------------------------------------------------------------------------------------------------
