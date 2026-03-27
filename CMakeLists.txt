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

# ____[OPTION NAME]_____________________[DESCRIPTION]___________________________________[DEFAULT]________________
option(EARENDIL_DISABLE_ALL             "Disables all Earendil libraries"               OFF)
# _______________________________________________________________________________________________________________

set(ENABLE_DEFAULT ON)
if (EARENDIL_DISABLE_ALL)
        set(ENABLE_DEFAULT OFF)
endif()

# ____[OPTION NAME]_____________________[DESCRIPTION]___________________________________[DEFAULT]________________
option(EARENDIL_RADIO            "Enables the Earendil_Radio subsystem"                 ${ENABLE_DEFAULT})
option(EARENDIL_GPS              "Enables the Earendil_GPS subsystem"                   ${ENABLE_DEFAULT})
option(EARENDIL_ALTIMETER        "Enables the Earendil_Altimeter subsystem"             ${ENABLE_DEFAULT})
option(EARENDIL_USDREADER        "Enables the Earendil_uSDReader subsystem"             ${ENABLE_DEFAULT})
option(EARENDIL_MAGNETOMETER     "Enables the Earendil_Magnetometer subsystem"          ${ENABLE_DEFAULT})
option(EARENDIL_DISPLAY          "Enables the Earendil_Display subsystem"               ${ENABLE_DEFAULT})
option(EARENDIL_ACCELGYRO        "Enables the Earendil_AccelGyro subsystem"             ${ENABLE_DEFAULT})
# _______________________________________________________________________________________________________________
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ·············································· EARENDIL LIBRARIES ·············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
# cmake/earendil/libraries
include(cmake/earendil/libraries/Earendil_Radio.cmake)
include(cmake/earendil/libraries/Earendil_GPS.cmake)
# include(cmake/earendil/libraries/Earendil_Altimeter.cmake)
# include(cmake/earendil/libraries/Earendil_uSDReader.cmake)
# include(cmake/earendil/libraries/Earendil_Magnetometer.cmake)
# include(cmake/earendil/libraries/Earendil_Display.cmake)
# include(cmake/earendil/libraries/Earendil_AccelGyro.cmake)
# cmake/earendil/compat
include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ············································ THIRD-PARTY LIBRARIES ············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
# cmake/earendil/libraries
include(cmake/third_party/libraries/RadioHead.cmake)
include(cmake/third_party/libraries/Adafruit_GPS.cmake)
# include(cmake/third_party/libraries/Adafruit_BMP3XX.cmake)
# include(cmake/third_party/libraries/SdFat.cmake)
# include(cmake/third_party/libraries/Adafruit_MMC56x3.cmake)
# include(cmake/third_party/libraries/Adafruit_GC9A01A.cmake)
# include(cmake/third_party/libraries/Adafruit_GFX_Library.cmake)
# include(cmake/third_party/libraries/Adafruit_LSM6DS.cmake)
# include(cmake/third_party/libraries/Adafruit_BusIO.cmake)
# include(cmake/third_party/libraries/Adafruit_Sensor.cmake)
# cmake/third_party/compat
include(cmake/third_party/compat/arduino_pico.cmake)
include(cmake/third_party/compat/ArduinoCore_API.cmake)
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ············································· CMAKE BUILD TARGETS  ············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
message(STATUS "\n")
message(STATUS "===== EARENDIL LIBRARIES // CONFIGURATION =====")
message(STATUS "EARENDIL_RADIO:                 ${EARENDIL_RADIO}")
message(STATUS "EARENDIL_GPS:                   ${EARENDIL_GPS}")
message(STATUS "EARENDIL_ALTIMETER:             ${EARENDIL_ALTIMETER}")
message(STATUS "EARENDIL_USDREADER:             ${EARENDIL_USDREADER}")
message(STATUS "EARENDIL_MAGNETOMETER:          ${EARENDIL_MAGNETOMETER}")
message(STATUS "EARENDIL_DISPLAY:               ${EARENDIL_DISPLAY}")
message(STATUS "EARENDIL_ACCELGYRO:             ${EARENDIL_ACCELGYRO}")

# EARENDIL // HANDHELD BUILD
set(VALID_OUTPUT_OPTS_HANDHELD ALL HANDHELD)
if (OUTPUT IN_LIST VALID_OUTPUT_OPTS_HANDHELD)
        add_executable(Earendil_Handheld ./src/FreeRTOS_Handheld.cpp)
        # Include FreeRTOS

        if (EARENDIL_RADIO)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Radio)
        endif()
        if (ENABLE_GPS)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_GPS)
        endif()
        if (ENABLE_ALTIMETER)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Altimeter)
        endif()
        if (ENABLE_USDREADER)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_uSDReader)
        endif()
        if (ENABLE_MAGNETOMETER)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Magnetometer)
        endif()
        if (ENABLE_DISPLAY)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Display)
        endif()
        if (ENABLE_ACCELGYRO)
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_AccelGyro)
        endif()
        
        target_compile_options(Earendil_Handheld PUBLIC 
                $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
                $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>
                $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
                $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
                $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
                $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
        )
        target_compile_definitions(Earendil_Handheld PUBLIC 
        # WARNING TEMPORARY --- WARNING TEMPORARY --- WARNING TEMPORARY
        PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
        )
        target_include_directories(FreeRTOS-Kernel INTERFACE ./config)

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
