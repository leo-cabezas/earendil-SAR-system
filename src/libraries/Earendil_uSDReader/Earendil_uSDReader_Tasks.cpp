#include <Earendil_uSDReader.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_uSDReader {

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
        createTask_vuSDReader();
    }

    // =======================================================================================
    // vuSDReader
    // =======================================================================================

    void createTask_vuSDReader(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->uSDReader_Handles.task_vuSDReader;
        xTaskCreate(
            vuSDReader,                                    // Task function
            "vuSDReader",                                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vuSDReader(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        vTaskDelay(pdMS_TO_TICKS(5000));
          
          while (1){
            //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // sleep until notified

            logData();

            vTaskDelay(pdMS_TO_TICKS(5000));
          }
    }
}
