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

include_guard()

add_library(Adafruit_GC9A01A STATIC
        ./third_party/libraries/Adafruit_GC9A01A/Adafruit_GC9A01A.cpp                       # Unmodified
)
target_compile_definitions(Adafruit_GC9A01A PUBLIC
        # None.
)
target_link_libraries(Adafruit_GC9A01A PUBLIC   # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # None.
)
target_link_libraries(Adafruit_GC9A01A PUBLIC   # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        arduino_pico
        Adafruit_GFX_Library
)
target_include_directories(Adafruit_GC9A01A PUBLIC
        ./third_party/libraries/Adafruit_GC9A01A
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_GC9A01A/Adafruit_GC9A01A.h
)

# ---------------------------------------------------------------------------------------------------------------
