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

# Toolchain default paths for Earendil
if (NOT DEFINED PICO_SDK_PATH)          # pico-sdk default path
        set(PICO_SDK_PATH               $ENV{HOME}/pico-sdk             CACHE STRING "pico-sdk default path for Earendil")
endif()
if (NOT DEFINED picotool_DIR)           # picotool default path
        set(picotool_DIR                $ENV{HOME}/picotool             CACHE STRING "picotool default path for Earendil")
endif()
if (NOT DEFINED FREERTOS_KERNEL_PATH)   # FreeRTOS-Kernel default path
        set(FREERTOS_KERNEL_PATH        $ENV{HOME}/FreeRTOS-Kernel      CACHE STRING "FreeRTOS-Kernel default path for Earendil")
endif()

set(CMAKE_C_STANDARD    11 CACHE STRING "C standard for Earendil")
set(CMAKE_CXX_STANDARD  17 CACHE STRING "C++ standard for Earendil")

if (NOT DEFINED CMAKE_BUILD_TYPE) # Use MinSizeRel to optimize for size (less RAM usage)!!!!!!!!!!!!!!!!!!
        set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type for Earendil")        
endif()

set(PICO_BOARD          adafruit_feather_rp2040)        # Target board for Earendil
set(PICO_PLATFORM       rp2040)                         # Target microcontroller platform for Earendil
set(PICO_COMPILER       pico_arm_cortex_m0plus_gcc)     # SDK compiler profile for the target board
set(PICO_GCC_TRIPLE     arm-none-eabi)                  # Cross compiler toolchain for Earendil

include(pico_sdk_import.cmake)          # Import include paths for pico-sdk
include(FreeRTOS_Kernel_import.cmake)   # Import include paths for FreeRTOS

project(Earendil C CXX ASM)     # CMake project declaration (C, C++, Assembly)

pico_sdk_init() # Initialize pico-sdk

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ················································ EARENDIL OPTIONS ·············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
set(VALID_OUTPUT_OPTS           ALL HANDHELD NODE)
set(VALID_OUTPUT_OPTS_HANDHELD  ALL HANDHELD)
set(VALID_OUTPUT_OPTS_NODE      ALL NODE)

if (NOT DEFINED OUTPUT)
        message(WARNING "OUTPUT was not defined, defaulting to ALL. Use -DOUTPUT=<option> (options: ${VALID_OUTPUT_OPTS}).")
        set(OUTPUT "ALL" CACHE STRING "Target output(s) (ALL, HANDHELD, NODE) for Earendil")
elseif (NOT OUTPUT IN_LIST VALID_OUTPUT_OPTS)
        message(FATAL_ERROR "Unknown OUTPUT: ${OUTPUT}. Please try again (options: ${VALID_OUTPUT_OPTS}).")
endif()

# ____[OPTION NAME]_____________[DESCRIPTION]___________________________________________[DEFAULT]________________
option(EARENDIL_DEFAULT     "Disables all Earendil libraries by default"            OFF)
# _______________________________________________________________________________________________________________

if (NOT DEFINED EARENDIL_DEFAULT)
        set(EARENDIL_DEFAULT ON CACHE BOOL "Enable/disable Earendil default features")
endif()

# ____[OPTION NAME]______________[DESCRIPTION]__________________________________________[DEFAULT]________________
option(EARENDIL_RADIO            "Enables the Earendil_Radio subsystem"                 ${EARENDIL_DEFAULT})
option(EARENDIL_GPS              "Enables the Earendil_GPS subsystem"                   ${EARENDIL_DEFAULT})
option(EARENDIL_ALTIMETER        "Enables the Earendil_Altimeter subsystem"             ${EARENDIL_DEFAULT})
option(EARENDIL_USDREADER        "Enables the Earendil_uSDReader subsystem"             ${EARENDIL_DEFAULT})
option(EARENDIL_MAGNETOMETER     "Enables the Earendil_Magnetometer subsystem"          ${EARENDIL_DEFAULT})
option(EARENDIL_DISPLAY          "Enables the Earendil_Display subsystem"               ${EARENDIL_DEFAULT})
option(EARENDIL_ACCELGYRO        "Enables the Earendil_AccelGyro subsystem"             ${EARENDIL_DEFAULT})
# _______________________________________________________________________________________________________________

message(STATUS "")
message(STATUS "===== EARENDIL LIBRARIES // CONFIGURATION =====")
message(STATUS "EARENDIL_RADIO:                 ${EARENDIL_RADIO}")
message(STATUS "EARENDIL_GPS:                   ${EARENDIL_GPS}")
message(STATUS "EARENDIL_ALTIMETER:             ${EARENDIL_ALTIMETER}")
message(STATUS "EARENDIL_USDREADER:             ${EARENDIL_USDREADER}")
message(STATUS "EARENDIL_MAGNETOMETER:          ${EARENDIL_MAGNETOMETER}")
message(STATUS "EARENDIL_DISPLAY:               ${EARENDIL_DISPLAY}")
message(STATUS "EARENDIL_ACCELGYRO:             ${EARENDIL_ACCELGYRO}")
message(STATUS "")
# ---------------------------------------------------------------------------------------------------------------

# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
# ===============================================================================================================
# ············································· CMAKE BUILD TARGETS  ············································
# ===============================================================================================================
# ///////////////////////////////////////////////////////////////////////////////////////////////////////////////

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // HANDHELD BUILD
if (OUTPUT IN_LIST VALID_OUTPUT_OPTS_HANDHELD)
        add_executable(Earendil_Handheld 
                ./src/FreeRTOS_Handheld.cpp
        )
        
        # Include FreeRTOS here (???)
        if (EARENDIL_ACCELGYRO STREQUAL "ON")
                message(STATUS "Including Earendil_AccelGyro and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_AccelGyro.cmake)

                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/Adafruit_LSM6DS.cmake)
                include(cmake/third_party/libraries/Adafruit_BusIO.cmake)
                include(cmake/third_party/libraries/Adafruit_Sensor.cmake)
                
                message(STATUS "Linking Earendil_AccelGyro module...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_AccelGyro)
        endif()
        if (EARENDIL_ALTIMETER STREQUAL "ON")
                message(STATUS "Including Earendil_Altimeter and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_Altimeter.cmake)

                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/Adafruit_BMP3XX.cmake)
                include(cmake/third_party/libraries/Adafruit_BusIO.cmake)
                

                message(STATUS "Linking Earendil_Altimeter module...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Altimeter)
        endif()
        if (EARENDIL_DISPLAY STREQUAL "ON")
                message(STATUS "Including Earendil_Display and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_Display.cmake)

                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/Adafruit_GC9A01A.cmake)
                include(cmake/third_party/libraries/Adafruit_GFX_Library.cmake)
                include(cmake/third_party/libraries/Adafruit_BusIO.cmake)

                message(STATUS "Linking Earendil_Display module...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Display)
        endif()
        if (EARENDIL_GPS STREQUAL "ON")
                message(STATUS "Including Earendil_GPS and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_GPS.cmake)

                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/Adafruit_GPS.cmake)

                message(STATUS "Linking Earendil_GPS library...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_GPS)
        endif()
        if (EARENDIL_MAGNETOMETER STREQUAL "ON")
                message(STATUS "Including Earendil_Magnetometer and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_Magnetometer.cmake)
                
                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/Adafruit_MMC56x3.cmake)
                include(cmake/third_party/libraries/Adafruit_BusIO.cmake)
                include(cmake/third_party/libraries/Adafruit_Sensor.cmake)

                message(STATUS "Linking Earendil_Magnetometer module...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Magnetometer)
        endif()
        if (EARENDIL_RADIO STREQUAL "ON")
                message(STATUS "Including Earendil_Radio and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_Radio.cmake)

                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/RadioHead.cmake)

                message(STATUS "Linking Earendil_Radio library...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_Radio)
        endif()
        if (EARENDIL_USDREADER STREQUAL "ON")
                message(STATUS "Including Earendil_uSDReader and its dependencies...")
                # cmake/earendil/compat/
                include(cmake/earendil/compat/Earendil_ArduinoCompat.cmake)
                # cmake/earendil/core/
                include(cmake/earendil/core/Earendil_TaskHandles.cmake)
                include(cmake/earendil/core/Earendil_SharedData.cmake)
                # cmake/earendil/libraries/
                include(cmake/earendil/libraries/Earendil_uSDReader.cmake)
                
                # cmake/third_party/compat/
                include(cmake/third_party/compat/arduino_pico.cmake)
                include(cmake/third_party/compat/ArduinoCore_API.cmake)
                # cmake/third_party/libraries/
                include(cmake/third_party/libraries/SdFat.cmake)               

                message(STATUS "Linking Earendil_uSDReader module...")
                target_link_libraries(Earendil_Handheld PUBLIC Earendil_uSDReader)
        endif()
        message(STATUS "")

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
        target_include_directories(FreeRTOS-Kernel INTERFACE 
                ./config
        )

        target_include_directories(Earendil_Handheld PUBLIC
                ./src
                ./src/libraries
        )

        # Enable USB, UART for debugging (TEMPORARY)
        pico_enable_stdio_usb(Earendil_Handheld 1)
        pico_enable_stdio_uart(Earendil_Handheld 1)
        # Generate .uf2 output format (needed to program RP2040 through FLASH memory) 
        pico_add_extra_outputs(Earendil_Handheld)
endif()

# EARENDIL // NODE BUILD
if (OUTPUT IN_LIST VALID_OUTPUT_OPTS_NODE)
        message(WARNING "Node not implemented yet.")
endif()
# ---------------------------------------------------------------------------------------------------------------

# End of CMakeLists.txt
