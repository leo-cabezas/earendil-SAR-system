####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# =========================== SPECIAL TERMINOLOGY ==============================
# Full:         The library was copied fully.
# Lean:         Only the minimum required source files have been copied from the original library.
# Unmodified:   Of those files copied, none were modified.
# Modified:     Some of the files copied were modified to work with Earendil.
# ==============================================================================

# Set CMake minimum version
cmake_minimum_required(VERSION 3.13)

# Import include paths for pico-sdk
include(pico_sdk_import.cmake)
include(FreeRTOS_Kernel_import.cmake)           # Import include paths for FreeRTOS

# Define C/C++ standards
project(example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize pico-sdk
pico_sdk_init()

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ················································· GLOBAL OPTIONS ··············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
set(VALID_OUTPUT_OPTS ALL HANDHELD NODE)
if (NOT DEFINED OUTPUT)
        message(WARNING "OUTPUT was not defined, defaulting to ALL. Use -DOUTPUT=<option> (options: ${VALID_OUTPUT_OPTS}).")
        set(OUTPUT "ALL")
elseif (NOT OUTPUT IN_LIST VALID_OUTPUT_OPTS)
        message(FATAL_ERROR "Unknown OUTPUT: ${OUTPUT}. Please try again (options: ${VALID_OUTPUT_OPTS}).")
endif()

# ____[OPTION NAME]_____________________[DESCRIPTION]________________________________________[DEFAULT]___________
option(EARENDIL_DISABLE_ALL             "Disables all Earendil libraries"                      OFF)
# _______________________________________________________________________________________________________________

set(ENABLE_DEFAULT ON)
if (EARENDIL_DISABLE_ALL)
        set(ENABLE_DEFAULT OFF)
endif()
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ·············································· EARENDIL LIBRARIES ·············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ____[OPTION NAME]_____________________[DESCRIPTION]___________________________________[DEFAULT]________________
# cmake/earendil/libraries
option(EARENDIL_RADIO            "Enables the Earendil_Radio subsystem"                 ${ENABLE_DEFAULT})
option(EARENDIL_GPS              "Enables the Earendil_GPS subsystem"                   ${ENABLE_DEFAULT})
option(EARENDIL_ALTIMETER        "Enables the Earendil_Altimeter subsystem"             ${ENABLE_DEFAULT})
option(EARENDIL_USDREADER        "Enables the Earendil_uSDReader subsystem"             ${ENABLE_DEFAULT})
option(EARENDIL_MAGNETOMETER     "Enables the Earendil_Magnetometer subsystem"          ${ENABLE_DEFAULT})
option(EARENDIL_DISPLAY          "Enables the Earendil_Display subsystem"               ${ENABLE_DEFAULT})
option(EARENDIL_ACCELGYRO        "Enables the Earendil_AccelGyro subsystem"             ${ENABLE_DEFAULT})
# cmake/earendil/compat
option(EARENDIL_ARDUINOCOMPAT    "Enables the Earendil_ArduinoCompat subsystem"         ${ENABLE_DEFAULT})
# _______________________________________________________________________________________________________________

# cmake/earendil/libraries
include(cmake/earendil/libraries/Earendil_Radio.cmake)
include(cmake/earendil/libraries/Earendil_GPS.cmake)
include(cmake/earendil/libraries/Earendil_Altimeter.cmake)
include(cmake/earendil/libraries/Earendil_uSDReader.cmake)
include(cmake/earendil/libraries/Earendil_Magnetometer.cmake)
include(cmake/earendil/libraries/Earendil_Display.cmake)
include(cmake/earendil/libraries/Earendil_AccelGyro.cmake)

# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ············································ THIRD-PARTY LIBRARIES ············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
# ____[OPTION NAME]_____________________[DESCRIPTION]________________________________________[DEFAULT]___________
# cmake/third_party/libraries
option(ENABLE_RADIOHEAD                 "Enables the RadioHead library"                         OFF)
option(ENABLE_ADAFRUIT_GPS              "Enables the Adafruit_GPS library"                      OFF)
option(ENABLE_ADAFRUIT_BMP3XX           "Enables the Adafruit_BMP3XX library"                   OFF)
option(ENABLE_SDFAT                     "Enables the SdFat library"                             OFF)
option(ENABLE_ADAFRUIT_MMC56X3          "Enables the Adafruit_MMC56x3 library"                  OFF)
option(ENABLE_ADAFRUIT_GC9A01A          "Enables the Adafruit_GC9A01A library"                  OFF)
option(ENABLE_ADAFRUIT_GFX_LIBRARY      "Enables the Adafruit-GFX-Library library"              OFF)
option(ENABLE_ADAFRUIT_LSM6DS           "Enables the Adafruit_LSM6DS library"                   OFF)
option(ENABLE_ADAFRUIT_BUSIO            "Enables the Adafruit_BusIO library"                    OFF)
option(ENABLE_ADAFRUIT_SENSOR           "Enables the Adafruit_Sensor library"                   OFF)
# cmake/third_party/compat
option(ENABLE_ARDUINO_PICO              "Enables the arduino-pico library"                      OFF)
option(ENABLE_ARDUINOCORE_API           "Enables the ArduinoCore-API library"                   OFF)
# _______________________________________________________________________________________________________________

# cmake/earendil/libraries
include(cmake/third_party/libraries/RadioHead.cmake)
include(cmake/third_party/libraries/Adafruit_GPS.cmake)
include(cmake/third_party/libraries/Adafruit_BMP3XX.cmake)
include(cmake/third_party/libraries/SdFat.cmake)
include(cmake/third_party/libraries/Adafruit_MMC56x3.cmake)
include(cmake/third_party/libraries/Adafruit_GC9A01A.cmake)
include(cmake/third_party/libraries/Adafruit_GFX_Library.cmake)
include(cmake/third_party/libraries/Adafruit_LSM6DS.cmake)
include(cmake/third_party/libraries/Adafruit_BusIO.cmake)
include(cmake/third_party/libraries/Adafruit_Sensor.cmake)
# cmake/third_party/compat
include(cmake/third_party/compat/arduino_pico.cmake)
include(cmake/third_party/compat/ArduinoCore_API.cmake)
# ---------------------------------------------------------------------------------------------------------------

# INCLUDES HAVE TO GO IN THE ORDER OF DEPENDENCY
# cmake/earendil/compat
include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)


# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ················································ FREERTOS SETUP ···············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
set(EARENDIL_FREERTOSCONFIG_HEADER_DIR
        ./config
)
set(EARENDIL_HANDHELD_ENTRY_SOURCE
        ./src/FreeRTOS_Handheld.cpp             # Original (attr. Leo Cabezas)
)
set(EARENDIL_NODE_ENTRY_SOURCE
        ./src/FreeRTOS_Node.cpp                 # Original (attr. Leo Cabezas)
)
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ············································· CMAKE BUILD TARGETS  ············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

set(GLOBAL_COMPILE_OPTS
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
)

# ---------------------------------------------------------------------------------------------------------------

message(STATUS "\n")
message(STATUS "===== EARENDIL LIBRARIES // CONFIGURATION =====")
# cmake/earendil/libraries
message(STATUS "EARENDIL_RADIO:                 ${EARENDIL_RADIO}")
message(STATUS "EARENDIL_GPS:                   ${EARENDIL_GPS}")
message(STATUS "EARENDIL_ALTIMETER:             ${EARENDIL_ALTIMETER}")
message(STATUS "EARENDIL_USDREADER:             ${EARENDIL_USDREADER}")
message(STATUS "EARENDIL_MAGNETOMETER:          ${EARENDIL_MAGNETOMETER}")
message(STATUS "EARENDIL_DISPLAY:               ${EARENDIL_DISPLAY}")
message(STATUS "EARENDIL_ACCELGYRO:             ${EARENDIL_ACCELGYRO}")
# cmake/earendil/compat
message(STATUS "EARENDIL_ARDUINOCOMPAT:         ${EARENDIL_ARDUINOCOMPAT}")
message(STATUS "\n")

message(STATUS "\n")
message(STATUS "===== THIRD-PARTY LIBRARIES // CONFIGURATION =====")
# cmake/third_party/libraries
message(STATUS "ENABLE_RADIOHEAD:               ${ENABLE_RADIOHEAD}")
message(STATUS "ENABLE_ADAFRUIT_GPS:            ${ENABLE_ADAFRUIT_GPS}")
message(STATUS "ENABLE_ADAFRUIT_BMP3XX:         ${ENABLE_ADAFRUIT_BMP3XX}")
message(STATUS "ENABLE_SDFAT:                   ${ENABLE_SDFAT}")
message(STATUS "ENABLE_ADAFRUIT_MMC56X3:        ${ENABLE_ADAFRUIT_MMC56X3}")
message(STATUS "ENABLE_ADAFRUIT_GC9A01A:        ${ENABLE_ADAFRUIT_GC9A01A}")
message(STATUS "ENABLE_ADAFRUIT_GFX_LIBRARY:    ${ENABLE_ADAFRUIT_GFX_LIBRARY}")
message(STATUS "ENABLE_ADAFRUIT_LSM6DS:         ${ENABLE_ADAFRUIT_LSM6DS}")
message(STATUS "ENABLE_ADAFRUIT_BUSIO:          ${ENABLE_ADAFRUIT_BUSIO}")
message(STATUS "ENABLE_ADAFRUIT_SENSOR:         ${ENABLE_ADAFRUIT_SENSOR}")
# cmake/third_party/compat
message(STATUS "ENABLE_ARDUINO_PICO:            ${ENABLE_ARDUINO_PICO}")
message(STATUS "ENABLE_ARDUINOCORE_API:         ${ENABLE_ARDUINOCORE_API}")
message(STATUS "\n")

# EARENDIL // HANDHELD BUILD
set(VALID_OUTPUT_OPTS_HANDHELD ALL HANDHELD)
if (OUTPUT IN_LIST VALID_OUTPUT_OPTS_HANDHELD)
        set(EARENDIL_HANDHELD_SOURCES
                # EARENDIL FREERTOS // SETUP
                ${EARENDIL_HANDHELD_ENTRY_SOURCE}
                # EARENDIL LIBRARIES // SOURCES
                ${EARENDIL_RADIO_SOURCES}
                ${EARENDIL_GPS_SOURCES}
                ${EARENDIL_ALTIMETER_SOURCES}
                ${EARENDIL_USDREADER_SOURCES}
                ${EARENDIL_MAGNETOMETER_SOURCES}
                ${EARENDIL_DISPLAY_SOURCES}
                ${EARENDIL_ACCELGYRO_SOURCES}
                ${EARENDIL_ARDUINOCOMPAT_SOURCES}
                # THIRD-PARTY LIBRARIES // SOURCES
                ${RADIOHEAD_SOURCES}
                ${ADAFRUIT_GPS_SOURCES}
                ${ADAFRUIT_BMP3XX_SOURCES}
                ${SDFAT_SOURCES}
                ${ADAFRUIT_MMC56X3_SOURCES}
                ${ADAFRUIT_GC9A01A_SOURCES}
                ${ADAFRUIT_GFX_LIBRARY_SOURCES}
                ${ADAFRUIT_LSM6DS_SOURCES}
                ${ADAFRUIT_BUSIO_SOURCES}
                ${ADAFRUIT_SENSOR_SOURCES}
                ${ARDUINO_PICO_SOURCES}
                ${ARDUINOCORE_API_SOURCES}
        )
        list(REMOVE_DUPLICATES EARENDIL_HANDHELD_SOURCES)
        add_executable(Earendil_Handheld ${EARENDIL_HANDHELD_SOURCES})
        
        set(EARENDIL_HANDHELD_COMPILE_OPTS
                ${GLOBAL_COMPILE_OPTS}
        )
        list(REMOVE_DUPLICATES EARENDIL_HANDHELD_COMPILE_OPTS)
        target_compile_options(Earendil_Handheld PUBLIC ${EARENDIL_HANDHELD_COMPILE_OPTS})

        set(EARENDIL_HANDHELD_COMPILE_DEFS
                # EARENDIL LIBRARIES // COMPILE DEFINITIONS
                ${EARENDIL_RADIO_COMPILE_DEFS}
                ${EARENDIL_GPS_COMPILE_DEFS}
                ${EARENDIL_ALTIMETER_COMPILE_DEFS}
                ${EARENDIL_USDREADER_COMPILE_DEFS}
                ${EARENDIL_MAGNETOMETER_COMPILE_DEFS}
                ${EARENDIL_DISPLAY_COMPILE_DEFS}
                ${EARENDIL_ACCELGYRO_COMPILE_DEFS}
                ${EARENDIL_ARDUINOCOMPAT_COMPILE_DEFS}
                # THIRD-PARTY LIBRARIES // COMPILE DEFINITIONS
                ${RADIOHEAD_COMPILE_DEFS}
                ${ADAFRUIT_GPS_COMPILE_DEFS}
                ${ADAFRUIT_BMP3XX_COMPILE_DEFS}
                ${SDFAT_COMPILE_DEFS}
                ${ADAFRUIT_MMC56X3_COMPILE_DEFS}
                ${ADAFRUIT_GC9A01A_COMPILE_DEFS}
                ${ADAFRUIT_GFX_LIBRARY_COMPILE_DEFS}
                ${ADAFRUIT_LSM6DS_COMPILE_DEFS}
                ${ADAFRUIT_BUSIO_COMPILE_DEFS}
                ${ADAFRUIT_SENSOR_COMPILE_DEFS}
                ${ARDUINO_PICO_COMPILE_DEFS}
                ${ARDUINOCORE_API_COMPILE_DEFS}
        )
        list(REMOVE_DUPLICATES EARENDIL_HANDHELD_COMPILE_DEFS)
        target_compile_definitions(Earendil_Handheld PRIVATE ${EARENDIL_HANDHELD_COMPILE_DEFS}
                
        # WARNING TEMPORARY --- WARNING TEMPORARY --- WARNING TEMPORARY
        PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
        )

        set(EARENDIL_HANDHELD_LINK_LIBS
                # EARENDIL LIBRARIES // LINK LIBRARIES
                ${EARENDIL_RADIO_LINK_LIBS}
                ${EARENDIL_GPS_LINK_LIBS}
                ${EARENDIL_ALTIMETER_LINK_LIBS}
                ${EARENDIL_USDREADER_LINK_LIBS}
                ${EARENDIL_MAGNETOMETER_LINK_LIBS}
                ${EARENDIL_DISPLAY_LINK_LIBS}
                ${EARENDIL_ACCELGYRO_LINK_LIBS}
                ${EARENDIL_ARDUINOCOMPAT_LINK_LIBS}
                # THIRD-PARTY LIBRARIES // LINK LIBRARIES
                ${RADIOHEAD_LINK_LIBS}
                ${ADAFRUIT_GPS_LINK_LIBS}
                ${ADAFRUIT_BMP3XX_LINK_LIBS}
                ${SDFAT_LINK_LIBS}
                ${ADAFRUIT_MMC56X3_LINK_LIBS}
                ${ADAFRUIT_GC9A01A_LINK_LIBS}
                ${ADAFRUIT_GFX_LIBRARY_LINK_LIBS}
                ${ADAFRUIT_LSM6DS_LINK_LIBS}
                ${ADAFRUIT_BUSIO_LINK_LIBS}
                ${ADAFRUIT_SENSOR_LINK_LIBS}
                ${ARDUINO_PICO_LINK_LIBS}
                ${ARDUINOCORE_API_LINK_LIBS}
        )
        list(REMOVE_DUPLICATES EARENDIL_HANDHELD_LINK_LIBS)
        target_link_libraries(Earendil_Handheld PRIVATE ${EARENDIL_HANDHELD_LINK_LIBS})
        
        set(EARENDIL_HANDHELD_HEADER_DIRS
                # EARENDIL FREERTOS // SETUP
                ${EARENDIL_FREERTOSCONFIG_HEADER_DIR}
                # EARENDIL LIBRARIES // HEADER DIRECTORIES
                ${EARENDIL_RADIO_HEADER_DIRS}
                ${EARENDIL_GPS_HEADER_DIRS}
                ${EARENDIL_ALTIMETER_HEADER_DIRS}
                ${EARENDIL_USDREADER_HEADER_DIRS}
                ${EARENDIL_MAGNETOMETER_HEADER_DIRS}
                ${EARENDIL_DISPLAY_HEADER_DIRS}
                ${EARENDIL_ACCELGYRO_HEADER_DIRS}
                ${EARENDIL_ARDUINOCOMPAT_HEADER_DIRS}
                # THIRD-PARTY LIBRARIES // HEADER DIRECTORIES
                ${RADIOHEAD_HEADER_DIRS}
                ${ADAFRUIT_GPS_HEADER_DIRS}
                ${ADAFRUIT_BMP3XX_HEADER_DIRS}
                ${SDFAT_HEADER_DIRS}
                ${ADAFRUIT_MMC56X3_HEADER_DIRS}
                ${ADAFRUIT_GC9A01A_HEADER_DIRS}
                ${ADAFRUIT_GFX_LIBRARY_HEADER_DIRS}
                ${ADAFRUIT_LSM6DS_HEADER_DIRS}
                ${ADAFRUIT_BUSIO_HEADER_DIRS}
                ${ADAFRUIT_SENSOR_HEADER_DIRS}
                ${ARDUINO_PICO_HEADER_DIRS}
                ${ARDUINOCORE_API_HEADER_DIRS}
        )
        list(REMOVE_DUPLICATES EARENDIL_HANDHELD_HEADER_DIRS)
        target_include_directories(Earendil_Handheld PRIVATE ${EARENDIL_HANDHELD_HEADER_DIRS})

        # Enable USB, UART for debugging (TEMPORARY)
        pico_enable_stdio_usb(Earendil_Handheld 1)
        pico_enable_stdio_uart(Earendil_Handheld 1)
        # Generate .uf2 output format (needed to program RP2040 through FLASH memory) 
        pico_add_extra_outputs(Earendil_Handheld)
endif()

# EARENDIL // NODE BUILD
set(VALID_OUTPUT_OPTS_NODE ALL NODE)
if (OUTPUT IN_LIST VALID_OUTPUT_OPTS_NODE)
        message(WARNING "Node not implemented yet.")
endif()
# ---------------------------------------------------------------------------------------------------------------

# End of CMakeLists.txt
