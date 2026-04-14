####################################################################
# Author: Leo Cabezas (University of Kansas)                       #
# Project: EARENDIL (Team 1; EECS 542)                             #
####################################################################

# ---------------------------------------------------------------------------------------------------------------
# EARENDIL // MUTEXES:
# ---> Name: Earendil_Mutexes
# ---> Metadata:
#         * Used in:    Handheld, Node
# ---> Dependencies:
#         * FreeRTOS-Kernel
# ---> Dependency of:
#         * 

include_guard()

add_library(Earendil_Mutexes INTERFACE
        # None.
)
target_compile_definitions(Earendil_Mutexes INTERFACE
        # None.
) 
target_link_libraries(Earendil_Mutexes INTERFACE    # Link all pico-sdk / FreeRTOS-Kernel dependencies.
        FreeRTOS-Kernel
        FreeRTOS-Kernel-Heap4
)
target_link_libraries(Earendil_Mutexes INTERFACE    # Link all non-(pico-sdk / FreeRTOS-Kernel) dependencies.
        # None.
)
target_include_directories(Earendil_Mutexes INTERFACE
        ./src/core/Earendil_Mutexes
        # Comprehensive header list for documentation purposes:
        # ./src/core/Earendil_Mutexes.h
)

# ---------------------------------------------------------------------------------------------------------------
