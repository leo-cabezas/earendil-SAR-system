#include <Earendil_AccelGyro.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_AccelGyro {
    Earendil::Earendil_TaskHandles_t*   Earendil_Handles    = nullptr; 
    Earendil::Earendil_SharedData_t*    Earendil_Data       = nullptr;

    void linkSharedStructs(
        Earendil::Earendil_TaskHandles_t*   global_Earendil_Handles,
        Earendil::Earendil_SharedData_t*    global_Earendil_Data
    ){
        Earendil_Handles    = global_Earendil_Handles;
        Earendil_Data       = global_Earendil_Data;
    }
    
    void createTasks(void){    
        createTask_vDisplay_NavScreen();
        createTask_vDisplay_MenuScreen();
        // createTask_vDisplay_Calibration();
    }

    // =======================================================================================
    // vAccelGyro_
    // =======================================================================================

}


void vAccelGyro(void* pvParameters){
  (void) pvParameters;
  vTaskDelay(pdMS_TO_TICKS(10000));
  printf("Accell-oscope started!\n");


  gyroSetup();
  // gyroCalibrate();
  
  while (1){
        // Normal operation: read and expose sensor data
        gyroReading();
        gyroShow(metrics); //For debugging
        vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

