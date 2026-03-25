####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- ACCELEROMETER + GYROSCOPE LIBRARIES: 
# ---> Name: Adafruit_LSM6DS
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_LSM6DS
#         * Version: 
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_LSM6DS_SOURCES             "")
set(ADAFRUIT_LSM6DS_COMPILE_DEFS        "")
set(ADAFRUIT_LSM6DS_LINK_LIBS           "")
set(ADAFRUIT_LSM6DS_HEADER_DIRS         "")

if (ENABLE_ADAFRUIT_LSM6DS)
set(ADAFRUIT_LSM6DS_SOURCES
        ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DS.cpp                         # Unmodified
        ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DSOX.cpp                       # Unmodified
)
set(ADAFRUIT_LSM6DS_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_LSM6DS_LINK_LIBS

)
set(ADAFRUIT_LSM6DS_HEADER_DIRS
        ./third_party/libraries/Adafruit_LSM6DS
)
endif()

# ---------------------------------------------------------------------------------------------------------------
