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

/*
void vAccelGyro(void* pvParameters){
  (void) pvParameters;
  vTaskDelay(pdMS_TO_TICKS(10000));
  printf("Accell-oscope started!\n");

  GyroMetrics_t metrics;
  EventBits_t   bits;
  gyroSetup();
  // gyroCalibrate();
  
  while (1){
    bits = xEventGroupGetBits(gyroEventGroup);

    if (bits & GYRO_EVT_CALIBRATE_REQUEST) {
        // Clear the request bit before starting
        xEventGroupClearBits(gyroEventGroup, GYRO_EVT_CALIBRATE_REQUEST);

        bool success = gyroCalibrate();

        // Signal completion or cancellation back to the requester
        xEventGroupSetBits(gyroEventGroup, success ? GYRO_EVT_CALIBRATE_COMPLETE : GYRO_EVT_CALIBRATE_CANCELLED);
    } else {
        // Normal operation: read and expose sensor data
        gyroReading(metrics);
        gyroShow(metrics); //For debugging
    }
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
*/
