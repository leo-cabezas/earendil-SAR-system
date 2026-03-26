####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY // PRECISION ALTIMETER LIBRARIES: 
# ---> Name: Adafruit_BMP3XX
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_BMP3XX
#         * Version: 
#         * License: 
#         * State:      Lean, Unmodified
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_BMP3XX_SOURCES             "")
set(ADAFRUIT_BMP3XX_COMPILE_DEFS        "")
set(ADAFRUIT_BMP3XX_LINK_LIBS           "")
set(ADAFRUIT_BMP3XX_HEADER_DIRS         "")

if (ENABLE_ADAFRUIT_BMP3XX)
set(ADAFRUIT_BMP3XX_SOURCES
        ./third_party/libraries/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.cpp                 # Unmodified
        ./third_party/libraries/Adafruit_BMP3XX_Library/bmp3.c                              # Unmodified
)
set(ADAFRUIT_BMP3XX_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_BMP3XX_LINK_LIBS

)
set(ADAFRUIT_BMP3XX_HEADER_DIRS
        ./third_party/libraries/Adafruit_BMP3XX_Library
)

endif()

# ---------------------------------------------------------------------------------------------------------------
