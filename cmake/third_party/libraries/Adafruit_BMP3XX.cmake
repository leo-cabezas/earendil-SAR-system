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

include_guard()

add_library(Adafruit_BMP3XX STATIC
        ./third_party/libraries/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.cpp                 # Unmodified
        ./third_party/libraries/Adafruit_BMP3XX_Library/bmp3.c                              # Unmodified
)
target_compile_definitions(Adafruit_BMP3XX PUBLIC
        # None.
)
target_link_libraries(Adafruit_BMP3XX PUBLIC    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_BMP3XX PUBLIC    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
        Adafruit_BusIO
)
target_include_directories(Adafruit_BMP3XX PUBLIC
        ./third_party/libraries/Adafruit_BMP3XX_Library
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.h
        # ./third_party/libraries/Adafruit_BMP3XX_Library/bmp3.h
        # ./third_party/libraries/Adafruit_BMP3XX_Library/bmp3_defs.h
)

# ---------------------------------------------------------------------------------------------------------------
