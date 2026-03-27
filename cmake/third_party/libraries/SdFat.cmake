####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# THIRD-PARTY // MICROSD CARD READER LIBRARIES:
# ---> Name: SdFat
# ---> Metadata:
#         * Source:     https://github.com/greiman/SdFat
#         * Version: 
#         * License: 
#         * State:
#         * Used in:    
# ---> Dependencies (libs):
#         * 
# ---> Dependency of (libs):
#         * 

set(SDFAT_SOURCES
        ./third_party/libraries/SdFat/src/FreeStack.cpp
        ./third_party/libraries/SdFat/src/MinimumSerial.cpp
        ./third_party/libraries/SdFat/src/common/FmtNumber.cpp
        ./third_party/libraries/SdFat/src/common/FsCache.cpp
        ./third_party/libraries/SdFat/src/common/FsDateTime.cpp
        ./third_party/libraries/SdFat/src/common/FsName.cpp
        ./third_party/libraries/SdFat/src/common/FsStructs.cpp
        ./third_party/libraries/SdFat/src/common/FsUtf.cpp
        ./third_party/libraries/SdFat/src/common/PrintBasic.cpp
        ./third_party/libraries/SdFat/src/common/upcase.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatDbg.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatFile.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatFilePrint.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatFileWrite.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatFormatter.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatName.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatPartition.cpp
        ./third_party/libraries/SdFat/src/ExFatLib/ExFatVolume.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatDbg.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatFile.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatFileLFN.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatFilePrint.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatFileSFN.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatFormatter.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatName.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatPartition.cpp
        ./third_party/libraries/SdFat/src/FatLib/FatVolume.cpp
        ./third_party/libraries/SdFat/src/FsLib/FsFile.cpp
        ./third_party/libraries/SdFat/src/FsLib/FsNew.cpp
        ./third_party/libraries/SdFat/src/FsLib/FsVolume.cpp
        ./third_party/libraries/SdFat/src/iostream/istream.cpp 
        ./third_party/libraries/SdFat/src/iostream/ostream.cpp
        ./third_party/libraries/SdFat/src/iostream/StdioStream.cpp
        ./third_party/libraries/SdFat/src/iostream/StreamBaseClass.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdCardInfo.cpp
        ./third_party/libraries/SdFat/src/SdCard/PioSdio/PioSdioCard.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SdSpiCard.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/SdSpiChipSelect.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/SdSpiDue.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/SdSpiSTM32Core.cpp
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/SdSpiTeensy3.cpp
        ./third_party/libraries/SdFat/src/SdCard/TeensySdio/TeensySdio.cpp
)
set(SDFAT_COMPILE_DEFS
        # None.
)
set(SDFAT_LINK_LIBS

)
set(SDFAT_HEADER_DIRS
        ./third_party/libraries/SdFat/src
        ./third_party/libraries/SdFat/src/common
        ./third_party/libraries/SdFat/src/ExFatLib
        ./third_party/libraries/SdFat/src/FatLib
        ./third_party/libraries/SdFat/src/FsLib
        ./third_party/libraries/SdFat/src/iostream
        ./third_party/libraries/SdFat/src/SdCard
        ./third_party/libraries/SdFat/src/SdCard/PioSdio
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/DigitalIO
        ./third_party/libraries/SdFat/src/SdCard/SdSpiCard/SpiDriver/DigitalIO/boards
        ./third_party/libraries/SdFat/src/SdCard/TeensySdio
)

# ---------------------------------------------------------------------------------------------------------------
