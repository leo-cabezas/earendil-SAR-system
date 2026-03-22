####################################################################
# Author: Leo Cabezas Amigo (University of Kansas)                 #
# Project: Eärendil (Team 1; EECS 542)                             #
####################################################################

# ====== SPECIAL MEANINGS ======
# Lean-ified: Only the minimum required source files have been copied from the original library.

# Set CMake minimum version
cmake_minimum_required(VERSION 3.13)

# Import include paths for pico-sdk
include(pico_sdk_import.cmake)
# Import include paths for FreeRTOS
include(FreeRTOS_Kernel_import.cmake)

# Define C/C++ standards
project(example C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize pico-sdk
pico_sdk_init()

# Include all source files for the 'earendil' executable
add_executable(earendil
        # Earendil: Project source files
                # FreeRTOS initializer source file
                ./src/freertos_init.cpp               # Original (attr. Leo)
                # Runnable OS tasks source files
                ./src/tasks/earendil_radio.cpp          # Original (just example code)
                ./src/tasks/earendil_gps.cpp            # Original (just example code)
                ./src/tasks/earendil_display.cpp        # Original (just example code)
                ./src/tasks/earendil_altimeter.cpp      # Original (just example code)
                ./src/tasks/earendil_accelgyro.cpp      # Original (just example code)
                # ./src/tasks/earendil_usdreader.cpp    # Original (just example code)
                ./src/tasks/earendil_magnetometer.cpp   # Original (just example code)
                
                # Compat stuff
                ./src/compat/earendil_arduino_compat.cpp        # Original (attr. Leo)
                                                
        # Third-party (libraries): RadioHead (version ; lean-ified)
                ./third_party/libraries/RadioHead/RH_RF95.cpp            # Unmodified (???) Probably modified
                ./third_party/libraries/RadioHead/RHSPIDriver.cpp        # Unmodified (???)
                ./third_party/libraries/RadioHead/RHGenericDriver.cpp    # Unmodified (???)
                ./third_party/libraries/RadioHead/RHHardwareSPI.cpp      # Unmodified (???) Most probably modified
                ./third_party/libraries/RadioHead/RHGenericSPI.cpp       # Unmodified (???)
        # Third-party (libraries): Adafruit_GPS_Library (version: ; license: ;lean-ified)
                ./third_party/libraries/Adafruit_GPS_Library/src/Adafruit_GPS.cpp        # Unmodified
                ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_build.cpp          # Unmodified
                ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_data.cpp           # Unmodified
                ./third_party/libraries/Adafruit_GPS_Library/src/NMEA_parse.cpp          # Unmodified
        # Third-party (libraries): Adafruit_GC9A01A (version: ; license: ; lean-ified)
                ./third_party/libraries/Adafruit_GC9A01A/Adafruit_GC9A01A.cpp        # Unmodified
        # Third-party (libraries): Adafruit_GFX_Library (version: ; license: ; lean-ified)
                ./third_party/libraries/Adafruit_GFX_Library/Adafruit_GFX.cpp        # Unmodified
                ./third_party/libraries/Adafruit_GFX_Library/Adafruit_SPITFT.cpp     # Unmodified
                ./third_party/libraries/Adafruit_GFX_Library/glcdfont.c              # Unmodified
        # Third-party (libraries): Adafruit_BusIO (version: ; license: ; lean-ified)
                # Used by Adafruit_GFX_Library (???), Adafruit_BMP3XX_Library, Adafruit_LSM6DS
                ./third_party/libraries/Adafruit_BusIO/Adafruit_BusIO_Register.cpp   # Unmodified
                ./third_party/libraries/Adafruit_BusIO/Adafruit_GenericDevice.cpp    # Unmodified
                ./third_party/libraries/Adafruit_BusIO/Adafruit_I2CDevice.cpp        # Unmodified
                ./third_party/libraries/Adafruit_BusIO/Adafruit_SPIDevice.cpp        # Unmodified
        # Third-party (libraries): Adafruit_BMP3XX_Library (version: ; license: ; lean-ified)
                ./third_party/libraries/Adafruit_BMP3XX_Library/Adafruit_BMP3XX.cpp         # Unmodified
                ./third_party/libraries/Adafruit_BMP3XX_Library/bmp3.c                      # Unmodified
        # Third-party (libraries): Adafruit_LSM6DS (version: ; license: ; lean-ified)
                ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DS.cpp        # Unmodified
                ./third_party/libraries/Adafruit_LSM6DS/Adafruit_LSM6DSOX.cpp      # Unmodified
        # Third-party (libraries): Adafruit_Unified_Sensor (version: ; license: ; lean-ified)
                # Used by Adafruit_LSM6DS, indirectly by Adafruit_BMP3XX_Library
                 ./third_party/libraries/Adafruit_Unified_Sensor/Adafruit_Sensor.cpp        # Unmodified
        # Third-party (libraries): SdFat (version: ; license: ; lean-ified)

        # Third-party (libraries): Adafruit_MMC56x3 (version: ; license: ; lean-ified)
                ./third_party/libraries/Adafruit_MMC56x3/Adafruit_MMC56x3.cpp        # Unmodified
        
        # Third-party (compat): arduino-pico (version: ; license: ; lean-ified)
                # cores/rp2040/api        THIS IS A DIFFERENT LIBRARY! ArduinoCore-API. NOTE IT SOMEWHERE
                # NOTE TO SELF: Check which files were modified and which weren't
                # Also need to somehow list all header files that were modified (down below!)
                ./third_party/compat/arduino-pico/cores/rp2040/api/Common.cpp        # Unmodified (???)
                ./third_party/compat/arduino-pico/cores/rp2040/api/Print.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/api/Stream.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/api/String.cpp
                # cores/rp2040
                ./third_party/compat/arduino-pico/cores/rp2040/_freertos.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/Bootsel.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/CoreMutex.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/PIOProgram.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/RP2040Support.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/SerialPIO.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/SerialUART.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/stdlib_noniso.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/delay.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/wiring_analog.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/wiring_digital.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/wiring_private.cpp
                ./third_party/compat/arduino-pico/cores/rp2040/WMath.cpp
                # libraries/SPI
                ./third_party/compat/arduino-pico/libraries/SPI/SPI.cpp
                # libraries/Wire
                ./third_party/compat/arduino-pico/libraries/Wire/Wire.cpp
)

# Include all header files for the 'earendil' executable
target_include_directories(earendil PRIVATE
        # Earendil: Project libraries
                # FreeRTOSConfig.h
                ./include
                # Earendil - Compatibility headers
                ./include/compat
                # Earendil - Task headers
                ./include/lib

        # Third-party (libraries): RadioHead (lean-ified) header directories
                ./third_party/libraries/RadioHead
        # Third-party (libraries): Adafruit_GPS_Library (lean-ified) header directories
                ./third_party/libraries/Adafruit_GPS_Library/src
        # Third-party (libraries): Adafruit_GC9A01A (lean-ified) header directories
                ./third_party/libraries/Adafruit_GC9A01A
        # Third-party (libraries): Adafruit_GFX_Library (lean-ified) header directories
                ./third_party/libraries/Adafruit_GFX_Library
        # Third-party (libraries): Adafruit_BusIO (lean-ified) header directories
                ./third_party/libraries/Adafruit_BusIO
        # Third-party (libraries): Adafruit_BMP3XX_Library (lean-ified) header directories
                ./third_party/libraries/Adafruit_BMP3XX_Library
        # Third-party (libraries): Adafruit_LSM6DS (lean-ified) header directories
                ./third_party/libraries/Adafruit_LSM6DS
        # Third-party (libraries): Adafruit_Unified_Sensor (lean-ified) header directories
                ./third_party/libraries/Adafruit_Unified_Sensor
        # Third-party (libraries): SdFat (lean-ified) header directories
                
        # Third-party (libraries): Adafruit_MMC56x3 (lean-ified) header directories
                ./third_party/libraries/Adafruit_MMC56x3
                
        # Third-party (compat): arduino-pico (version ; lean-ifed) header directories
                # cores/rp2040
                ./third_party/compat/arduino-pico/cores/rp2040
                # cores/rp2040/api
                ./third_party/compat/arduino-pico/cores/rp2040/api
                # variants/adafruit_feather_rfm
                ./third_party/compat/arduino-pico/variants/adafruit_feather_rfm
                # libraries/SPI
                ./third_party/compat/arduino-pico/libraries/SPI
                # libraries/Wire
                ./third_party/compat/arduino-pico/libraries/Wire
)

# Set compile-time project-level definitions
target_compile_definitions(earendil PRIVATE
        # --- RadioHead library compile definitions ---
        ARDUINO=20308
        RH_ARDUINO_ARCH_RP2040
        # TEMPORARY
        PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
)

# Link all relevant project libraries (pico-sdk libs, SPI, I2C, FreeRTOS, etc.)
target_link_libraries(earendil
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

# Generate .uf2 output format (needed to program RP2040 through FLASH memory) 
pico_add_extra_outputs(earendil)

# Enable USB, UART for debugging (TEMPORARY)
pico_enable_stdio_usb(earendil 1)
pico_enable_stdio_uart(earendil 1)

# Set compile options (TEMPORARY)
target_compile_options(earendil PUBLIC
        # Gnu/Clang C Options
        $<$<COMPILE_LANG_AND_ID:C,GNU>:-fdiagnostics-color=always>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-fcolor-diagnostics>

        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wall>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Wextra>
        $<$<COMPILE_LANG_AND_ID:C,Clang,GNU>:-Werror>
        $<$<COMPILE_LANG_AND_ID:C,Clang>:-Weverything>
)
