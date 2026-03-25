####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- COMMON LIBRARIES:      
# ---> Name: Adafruit_Sensor
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_Sensor
#         * Version: 
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_SENSOR_SOURCES             "")
set(ADAFRUIT_SENSOR_COMPILE_DEFS        "")
set(ADAFRUIT_SENSOR_LINK_LIBS           "")
set(ADAFRUIT_SENSOR_HEADER_DIRS         "")

if (ENABLE_ADAFRUIT_SENSOR)
set(ADAFRUIT_SENSOR_SOURCES
        ./third_party/libraries/Adafruit_Unified_Sensor/Adafruit_Sensor.cpp                 # Unmodified
)
set(ADAFRUIT_SENSOR_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_SENSOR_LINK_LIBS

)
set(ADAFRUIT_SENSOR_HEADER_DIRS
        ./third_party/libraries/Adafruit_Unified_Sensor
)
endif()

# ---------------------------------------------------------------------------------------------------------------
