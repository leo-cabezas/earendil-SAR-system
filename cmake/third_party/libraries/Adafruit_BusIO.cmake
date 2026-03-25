####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- COMMON LIBRARIES:
# ---> Name: Adafruit_BusIO
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_BusIO
#         * Version: 
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_BUSIO_SOURCES              "")
set(ADAFRUIT_BUSIO_COMPILE_DEFS         "")
set(ADAFRUIT_BUSIO_LINK_LIBS            "")
set(ADAFRUIT_BUSIO_HEADER_DIRS          "")

if(ENABLE_ADAFRUIT_BUSIO)
set(ADAFRUIT_BUSIO_SOURCES
        ./third_party/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.cpp                  # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_GenericDevice.cpp                   # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_I2CDevice.cpp                       # Unmodified
        ./third_party/libraries/Adafruit_BusIO/Adafruit_SPIDevice.cpp                       # Unmodified
)
set(ADAFRUIT_BUSIO_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_BUSIO_LINK_LIBS

)
set(ADAFRUIT_BUSIO_HEADER_DIRS
        ./third_party/libraries/Adafruit_BusIO
)
endif()

# ---------------------------------------------------------------------------------------------------------------
