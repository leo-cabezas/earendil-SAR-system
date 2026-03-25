####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ROUND LCD DISPLAY LIBRARIES:
# ---> Name: Adafruit-GFX-Library
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit-GFX-Library
#         * Version: 
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_GFX_LIBRARY_SOURCES        "")
set(ADAFRUIT_GFX_LIBRARY_COMPILE_DEFS   "")
set(ADAFRUIT_GFX_LIBRARY_LINK_LIBS      "")
set(ADAFRUIT_GFX_LIBRARY_HEADER_DIRS    "")

if(ENABLE_ADAFRUIT_GFX_LIBRARY)
set(ADAFRUIT_GFX_LIBRARY_SOURCES
        ./third_party/libraries/Adafruit_GFX_Library/Adafruit_GFX.cpp                       # Unmodified
        ./third_party/libraries/Adafruit_GFX_Library/Adafruit_SPITFT.cpp                    # Unmodified
        ./third_party/libraries/Adafruit_GFX_Library/glcdfont.c                             # Unmodified
)
set(ADAFRUIT_GFX_LIBRARY_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_GFX_LIBRARY_LINK_LIBS

)
set(ADAFRUIT_GFX_LIBRARY_HEADER_DIRS
        ./third_party/libraries/Adafruit_GFX_Library
)
endif()

# ---------------------------------------------------------------------------------------------------------------
