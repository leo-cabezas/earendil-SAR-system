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

set(ADAFRUIT_GPS_SOURCES        "")
set(ADAFRUIT_GPS_COMPILE_DEFS   "")
set(ADAFRUIT_GPS_LINK_LIBS      "")
set(ADAFRUIT_GPS_HEADER_DIRS    "")

if (ENABLE_ADAFRUIT_GPS)
set(ADAFRUIT_GPS_SOURCES
        ./third_party/libraries/Adafruit_GPS_Library/src/Adafruit_GPS.cpp                   # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_build.cpp                     # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_data.cpp                      # Unmodified
        ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_parse.cpp                     # Unmodified
)
set(ADAFRUIT_GPS_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_GPS_LINK_LIBS

)
set(ADAFRUIT_GPS_HEADER_DIRS
        ./third_party/libraries/Adafruit_GPS_Library/src
)

endif()

# ---------------------------------------------------------------------------------------------------------------
