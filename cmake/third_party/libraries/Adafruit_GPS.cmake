####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY // GNSS (GPS) RECEIVER LIBRARIES:
# ---> Name: Adafruit_GPS
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_GPS
#         * Version:    1.7.5
#         * License:    BSD License
#         * State:      Lean, Unmodified
#         * Used in:    Handheld, Node
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

add_library(Adafruit_GPS STATIC
        ./third_party/libraries/Adafruit_GPS_Library/src/Adafruit_GPS.cpp                   # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_build.cpp                     # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_data.cpp                      # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_parse.cpp                     # Unmodified
)
target_compile_definitions(Adafruit_GPS PUBLIC
        # None.
)
target_link_libraries(Adafruit_GPS PUBLIC       # Link all pico-sdk / FreeRTOS-Kernel dependencies
        # None.
)
target_link_libraries(Adafruit_GPS PUBLIC       # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies
        arduino_pico
)
target_include_directories(Adafruit_GPS PUBLIC
        ./third_party/libraries/Adafruit_GPS_Library/src
        # Comprehensive header list for documentation purposes:
        # ./third_party/libraries/Adafruit_GPS_Library/src/Adafruit_GPS.h
        # ./third_party/libraries/Adafruit_GPS_Library/src/Adafruit_PMTK.h
        # ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_data.h
)

# ---------------------------------------------------------------------------------------------------------------
