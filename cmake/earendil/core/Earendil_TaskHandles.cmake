####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // TASK HANDLES:
# ---> Name: Earendil_TaskHandles
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * 

include_guard()

add_library(Earendil_TaskHandles INTERFACE
        # None.
)
target_compile_definitions(Earendil_TaskHandles INTERFACE
        # None.
) 
target_link_libraries(Earendil_TaskHandles INTERFACE    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)
target_link_libraries(Earendil_TaskHandles INTERFACE    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # None.
)
target_include_directories(Earendil_TaskHandles INTERFACE
        ./src/core/Earendil_TaskHandles
        ./src/libraries/Earendil_AccelGyro/Earendil_AccelGyro_TaskHandles
        ./src/libraries/Earendil_Altimeter/Earendil_Altimeter_TaskHandles
        ./src/libraries/Earendil_Display/Earendil_Display_TaskHandles
        ./src/libraries/Earendil_GPS/Earendil_GPS_TaskHandles
        ./src/libraries/Earendil_Magnetometer/Earendil_Magnetometer_TaskHandles
        ./src/libraries/Earendil_Radio/Earendil_Radio_TaskHandles
        ./src/libraries/Earendil_uSDReader/Earendil_uSDReader_TaskHandles
        # Comprehensive header list for documentation purposes:
        # List here...
)

# ---------------------------------------------------------------------------------------------------------------
