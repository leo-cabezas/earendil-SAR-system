####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // SHARED DATA:
# ---> Name: Earendil_SharedData
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * 

include_guard()

add_library(Earendil_SharedData INTERFACE
        # None.
)
target_compile_definitions(Earendil_SharedData INTERFACE
        # None.
) 
target_link_libraries(Earendil_SharedData INTERFACE     # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        # FreeRTOS-Kernel
        # FreeRTOS-Kernel-Heap4
)
target_link_libraries(Earendil_SharedData INTERFACE     # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # None.
)
target_include_directories(Earendil_SharedData INTERFACE
        ./src/core/Earendil_SharedData
        ./src/libraries/Earendil_AccelGyro/Earendil_AccelGyro_SharedData
        ./src/libraries/Earendil_Altimeter/Earendil_Altimeter_SharedData
        # ./src/libraries/Earendil_Display/Earendil_Display_SharedData
        ./src/libraries/Earendil_GPS/Earendil_GPS_SharedData
        ./src/libraries/Earendil_Magnetometer/Earendil_Magnetometer_SharedData
        ./src/libraries/Earendil_Radio/Earendil_Radio_SharedData
        ./src/libraries/Earendil_uSDReader/Earendil_uSDReader_SharedData
        # Comprehensive header list for documentation purposes:
        # List here...
)

# ---------------------------------------------------------------------------------------------------------------
