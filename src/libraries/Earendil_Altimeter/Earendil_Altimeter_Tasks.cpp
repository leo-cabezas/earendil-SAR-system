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
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vAltimeter;
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
        
<<<<<<< HEAD
        if (!bmp.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
            printf("Could not find a valid BMP3 sensor, check wiring!");
            vTaskDelete(NULL);
        }

        // Set up oversampling and filter initialization
        bmp.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
        bmp.setPressureOversampling(BMP3_OVERSAMPLING_16X);
        bmp.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
        bmp.setOutputDataRate(BMP3_ODR_1_5_HZ);

        SensorData_t temperature;
        temperature.eSource = altTemp;
        SensorData_t pressure;
        pressure.eSource = altPress;

        while(1){
            if (!bmp.performReading()) {
                printf("Failed to perform reading :(");
                vTaskDelete(NULL);
            }

            temperature.eData = bmp.temperature;
            pressure.eData = bmp.pressure;

            if (xQueueSend(auQueue, &temperature, 0) != pdPASS){
                printf("Failed to send temperature.");
                vTaskDelete(NULL);
            }
            if (xQueueSend(auQueue, &pressure, 0) != pdPASS){
                printf("Failed to send pressure.");
                vTaskDelete(NULL);
            }

            vTaskDelay(pdMS_TO_TICKS(5000));
        }
=======
        vTaskDelay(pdMS_TO_TICKS(10000));
          
          while (1){

            altRead_hh();//Handheld
            //altRead_node();//Node

            vTaskDelay(pdMS_TO_TICKS(5000));  
          }
>>>>>>> 57615aab13e960a6adf6554990b17741fa58444b
    }
}
