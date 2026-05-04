#include <Earendil_AccelGyro.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_AccelGyro {
    Earendil::Earendil_TaskHandles_t*   Earendil_Handles    = nullptr; 
    Earendil::Earendil_SharedData_t*    Earendil_Data       = nullptr;
    Earendil::Earendil_Mutexes_t*       Earendil_Mutexes    = nullptr;

    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data,
        Earendil::Earendil_Mutexes_t*       global_Earendil_Mutexes
    ){
        Earendil_Handles    = global_Earendil_Handles;
        Earendil_Data       = global_Earendil_Data;
        Earendil_Mutexes    = global_Earendil_Mutexes;
    }
    
    void createTasks(void){    
        createTask_vAccelGyro();
    }

    // =======================================================================================
    // vAccelGyro_
    // =======================================================================================


    void createTask_vAccelGyro(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->AccelGyro_Handles.task_vAccelGyro;
        xTaskCreate(
            vAccelGyro,                    // Task function
            "vAccelGyro",                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            1                                               // Assigned core (options: 0, 1)
        );
    }   

    void vAccelGyro(void* pvParameters){
        (void) pvParameters;
        TickType_t xLastWakeTime;
        const TickType_t xFrequency = pdMS_TO_TICKS(DELAY_VACCELGYRO);
        BaseType_t xWasDelayed;
        
        xLastWakeTime = xTaskGetTickCount();
        //printf("Accell-oscope started!\n");

        gyroCalibrate();
        
        while (1){
            // Normal operation: read and expose sensor data
            //gyroCalibrate();
            gyroReading();
            //gyroShow(); //For debugging
            xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

}