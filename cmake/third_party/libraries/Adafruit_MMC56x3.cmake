####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY --- MAGNETOMETER LIBRARIES: 
# ---> Name: Adafruit_MMC56x3
# ---> Metadata:
#         * Source:     https://github.com/adafruit/Adafruit_MMC56x3
#         * Version:    
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(ADAFRUIT_MMC56X3_SOURCES            "")
set(ADAFRUIT_MMC56X3_COMPILE_DEFS       "")
set(ADAFRUIT_MMC56X3_LINK_LIBS          "")
set(ADAFRUIT_MMC56X3_HEADER_DIRS        "")

if (ENABLE_ADAFRUIT_MMC56X3)
set(ADAFRUIT_MMC56X3_SOURCES
        ./third_party/libraries/Adafruit_MMC56x3/Adafruit_MMC56x3.cpp                       # Unmodified
)
set(ADAFRUIT_MMC56X3_COMPILE_DEFS
        # None.
)
set(ADAFRUIT_MMC56X3_LINK_LIBS

)
set(ADAFRUIT_MMC56X3_HEADER_DIRS
        ./third_party/libraries/Adafruit_MMC56x3
)

endif()

# ---------------------------------------------------------------------------------------------------------------
