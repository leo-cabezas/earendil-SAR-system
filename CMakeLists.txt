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
include(cmake/pico_sdk_import.cmake)

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
if(NOT DEFINED OUTPUT)
        message(WARNING "OUTPUT was not defined, defaulting to ALL. Use -DOUTPUT=<option> (options: ${VALID_OUTPUT_OPTS}).")
        set(OUTPUT "ALL")
elseif(NOT OUTPUT IN_LIST VALID_OUTPUT_OPTS)
        message(FATAL_ERROR "Unknown OUTPUT: ${OUTPUT}. Please try again (options: ${VALID_OUTPUT_OPTS}).")
endif()

# ____[OPTION NAME]_____________[DESCRIPTION]________________________________________________[DEFAULT]___________
# option(ENABLE_RADIO             "Enables the Radio subsystem"                                   ON)
# option(ENABLE_GPS               "Enables the GPS subsystem"                                     ON)
# option(ENABLE_ALTIMETER         "Enables the Altimeter subsystem"                               ON)
# option(ENABLE_USDREADER         "Enables the MicroSD Reader subsystem"                          ON)
# option(ENABLE_MAGNETOMETER      "Enables the Magnetometer subsystem"                            ON)
# option(ENABLE_DISPLAY           "Enables the Display subsystem"                                 ON)
# option(ENABLE_ACCELGYRO         "Enables the Accelerometer + Gyroscope subsystem"               ON)

# _______________________________________________________________________________________________________________
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ·············································· EARENDIL LIBRARIES ·············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
# ____[OPTION NAME]_____________________[DESCRIPTION]________________________________________[DEFAULT]___________
# cmake/earendil/libraries
option(ENABLE_EARENDIL_RADIO            "Enables the Earendil_Radio library"                    ON)
option(ENABLE_EARENDIL_GPS              "Enables the Earendil_GPS library"                      ON)
option(ENABLE_EARENDIL_ALTIMETER        "Enables the Earendil_Altimeter library"                ON)
option(ENABLE_EARENDIL_USDREADER        "Enables the Earendil_uSDReader library"                ON)
option(ENABLE_EARENDIL_MAGNETOMETER     "Enables the Earendil_Magnetometer library"             ON)
option(ENABLE_EARENDIL_DISPLAY          "Enables the Earendil_Display library"                  ON)
option(ENABLE_EARENDIL_ACCELGYRO        "Enables the Earendil_AccelGyro library"                ON)
# cmake/earendil/compat
option(ENABLE_EARENDIL_ARDUINOCOMPAT    "Enables the Earendil_ArduinoCompat library"            ON)
# _______________________________________________________________________________________________________________

# cmake/earendil/libraries
include(cmake/earendil/libraries/Earendil_Radio.cmake)
include(cmake/earendil/libraries/Earendil_GPS.cmake)
include(cmake/earendil/libraries/Earendil_Altimeter.cmake)
include(cmake/earendil/libraries/Earendil_uSDReader.cmake)
include(cmake/earendil/libraries/Earendil_Magnetometer.cmake)
include(cmake/earendil/libraries/Earendil_Display.cmake)
include(cmake/earendil/libraries/Earendil_AccelGyro.cmake)
# cmake/earendil/compat
include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
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

message(STATUS "===== Build Configuration =====")
message(STATUS "OUTPUT: ${OUTPUT}")
message(STATUS )
message(STATUS "---> RADIO:  ${ENABLE_EARENDIL_RADIO}")
message(STATUS "================================")
message(STATUS "")

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ················································ FREERTOS SETUP ···············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
include(cmake/FreeRTOS_Kernel_import.cmake)           # Import include paths for FreeRTOS

set(EARENDIL_FREERTOSCONFIG_HEADER_DIR
        ./include
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

set(COMPILE_OPTS
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
)

set(LINK_LIBS
        # Link relevant pico-sdk libraries
        pico_stdlib # Actually an aggregate of libraries. CHECK OUT LATER ON!!!

        pico_rand
        
        hardware_dma
        hardware_spi
        hardware_gpio
        hardware_irq
        hardware_sync
        hardware_clocks
        hardware_pio
        hardware_exception
        hardware_watchdog
        hardware_claim
        hardware_uart
        hardware_i2c
        hardware_pwm
        hardware_adc
        # hardware_flash
        
        # Link relevant FreeRTOS libraries
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)

# ---------------------------------------------------------------------------------------------------------------
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
                ${COMPILE_OPTS}
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
                # !!!!!!!!!!!!! TEMPORARY !!!!!!!!!!!!!!
                ${LINK_LIBS}

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
