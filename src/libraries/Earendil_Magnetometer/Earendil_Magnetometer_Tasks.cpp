#include <Earendil_Magnetometer.h>  // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Magnetometer {

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
        createTask_vMagnetometer_UpdateHeading();
        createTask_vMagnetometer_Calibrate();
    }

    // =======================================================================================
    // vMagnetometer_UpdateHeading
    // =======================================================================================

    void createTask_vMagnetometer_UpdateHeading(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Magnetometer_Handles.task_vMagnetometer_UpdateHeading;
        xTaskCreate(
            vMagnetometer_UpdateHeading,                    // Task function
            "vMagnetometer_UpdateHeading",                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            2,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            1                                               // Assigned core (options: 0, 1)
        );
    }
    
    void vMagnetometer_UpdateHeading(void* pvParameters){
        (void)pvParameters;     // Parameters unused.


        TickType_t xLastWakeTime;
        const TickType_t xFrequency = pdMS_TO_TICKS(DELAY_VMAGNETOMETER);
        BaseType_t xWasDelayed;
        
        xLastWakeTime = xTaskGetTickCount();

        sensors_event_t event;
        double raw[3] = {0, 0, 0};

        while (1){
            magnetometer.getEvent(&event);

            raw[0] = event.magnetic.x;
            raw[1] = event.magnetic.y;
            raw[2] = event.magnetic.z;

            //char buf[64];
            //snprintf(buf, sizeof(buf), "raw_x=%.2f\nraw_y=%.2f\nraw_z=%.2f\n", raw[0], raw[1], raw[2]);
            //puts(buf);
            
            updateFilter(raw);
            double calibrated[3];
            applyCalibration(calibrated);

            taskENTER_CRITICAL();
            float ax = Earendil_Data->AccelGyro_Data.GyroMetrics[0];
            float ay = Earendil_Data->AccelGyro_Data.GyroMetrics[1];
            float az = Earendil_Data->AccelGyro_Data.GyroMetrics[2];
            taskEXIT_CRITICAL();

            Earendil_Data->Magnetometer_Data.heading_deg = getHeading(calibrated, ax, ay, az);

            xWasDelayed = xTaskDelayUntil(&xLastWakeTime, xFrequency);
        }
    }

    // =======================================================================================
    // vMagnetometer_Calibrate
    // =======================================================================================

    
    void createTask_vMagnetometer_Calibrate(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Magnetometer_Handles.task_vMagnetometer_Calibrate;
        xTaskCreate(
            vMagnetometer_Calibrate,                    // Task function
            "vMagnetometer_Calibrate",                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            1                                               // Assigned core (options: 0, 1)
        );
    }

    void vMagnetometer_Calibrate(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        while (1){
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            calibrateMagnetometer();
            xTaskNotifyGive(Earendil_Handles->Display_Handles.task_vDisplay_Controls);
        }
    }
}