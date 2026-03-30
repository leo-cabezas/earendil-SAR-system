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
#         * State:      Lean, Unmodified
#         * Used in:    Handheld
# ---> Dependencies (libs):
#         * arduino-pico
# ---> Dependency of (libs):
#         * Adafruit_GC9A01A

include_guard()

add_library(Adafruit_GFX_Library STATIC
        ./third_party/libraries/Adafruit_GFX_Library/Adafruit_GFX.cpp                   # Unmodified
        ./third_party/libraries/Adafruit_GFX_Library/Adafruit_SPITFT.cpp                # Unmodified
        ./third_party/libraries/Adafruit_GFX_Library/glcdfont.c                         # Unmodified
)
target_compile_definitions(Adafruit_GFX_Library PUBLIC
        ARDUINO=20308                           # Dummy Arduino version (anything >= 100 would work).
)
target_link_libraries(Adafruit_GFX_Library PUBLIC       # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_GFX_Library PUBLIC       # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
        Adafruit_BusIO
)
target_include_directories(Adafruit_GFX_Library PUBLIC
        ./third_party/libraries/Adafruit_GFX_Library
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_GFX_Library/Adafruit_GFX.h                   # Unmodified  
        # ./third_party/libraries/Adafruit_GFX_Library/Adafruit_SPITFT.h                # Unmodified
        # ./third_party/libraries/Adafruit_GFX_Library/Adafruit_SPITFT_Macros.h         # Unmodified
        # ./third_party/libraries/Adafruit_GFX_Library/glxfont.h                        # Unmodified
)

# ---------------------------------------------------------------------------------------------------------------
