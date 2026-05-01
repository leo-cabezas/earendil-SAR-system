#include <Earendil_Altimeter.h>   // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Altimeter {

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
        createTask_vAltimeter();
    }

    // =======================================================================================
    // vAltimeter
    // =======================================================================================

    void createTask_vAltimeter(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Altimeter_Handles.task_vAltimeter;
        xTaskCreate(
            vAltimeter,                                    // Task function
            "vAltimeter",                                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vAltimeter(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        TickType_t xLastWakeTime;
        const TickType_t xFrequency = pdMS_TO_TICKS(DELAY_VDISPLAY_UI);
        BaseType_t xWasDelayed;
        
        xLastWakeTime = xTaskGetTickCount();

        vTaskDelay(pdMS_TO_TICKS(10000));
          
          while (1){

            //altRead_hh();//Handheld
            altRead_node();//Node

            xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
          }
    }
}
