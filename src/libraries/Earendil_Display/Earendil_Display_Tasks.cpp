#include <Earendil_Display.h>   // ATTENTION: Add all library dependencies inside this header.

namespace Earendil_Display {
    
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
        createTask_vDisplay_NavControl();
        createTask_vDisplay_MenuControl();
        // createTask_vDisplay_Calibration();
    }

    // =======================================================================================
    // vDisplay_NavScreen
    // =======================================================================================

    void createTask_vDisplay_NavScreen(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_NavScreen;
        xTaskCreate(
            vDisplay_NavScreen,                             // Task function
            "vDisplay_NavScreen",                           // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_NavScreen(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        while(1){
            displayNav();

            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
    // =======================================================================================
    // vDisplay_NavControl
    // =======================================================================================

    void createTask_vDisplay_NavControl(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_NavControl;
        xTaskCreate(
            vDisplay_NavControl,                             // Task function
            "vDisplay_NavControl",                           // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_NavControl(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        while(1){
            controlNav();
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
    // =======================================================================================
    // vDisplay_MenuScreen
    // =======================================================================================

    // !!!!!!!!! MAYBE DIVIDE TASK INTO TWO: UPDATE & BUTTON CONTROL !!!!!!!!!!!!!!!

    void createTask_vDisplay_MenuScreen(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_MenuScreen;
        xTaskCreate(
            vDisplay_MenuScreen,                             // Task function
            "vDisplay_MenuScreen",                           // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_MenuScreen(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        vTaskSuspend(NULL); // Initially self-suspend.

        setupMenu();
        
        while(1){
            drawMenu();
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }

    
// =======================================================================================
    // vDisplay_MenuControl
    // =======================================================================================

    void createTask_vDisplay_MenuControl(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_MenuControl;
        xTaskCreate(
            vDisplay_MenuControl,                             // Task function
            "vDisplay_MenuControl",                           // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_MenuControl(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        vTaskSuspend(NULL);

        while(1){
            menuControl();
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }


    // =======================================================================================
    // vDisplayCalibration
    // =======================================================================================

    /*
    void createTask_vDisplay_Calibration(void){
        TaskHandle_t* task_handle_ptr = Earendil_Handles->Display_Handles->task_vDisplay_Calibration;
        xTaskCreate(
            vDisplayCalibration,                            // Task function
            "vDisplayCalibration",                          // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplayCalibration(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        vTaskSuspend(NULL);
        
        display.fillScreen(GC9A01A_BLACK);
        display.setCursor(X_MENU_OFFSET, Y_MENU_OFFSET);
        display.setTextSize(1.5);
        display.print("Gyro Calibration");
        EventBits_t bits;
        display.setCursor(X_OFFSET-50, Y_OFFSET);
        vTaskDelay(pdMS_TO_TICKS(50));

        xEventGroupSetBits(gyroEventGroup, GYRO_EVT_CALIBRATE_REQUEST); //This is breaking for some reason

        while(1){
            display.print("IN while loop");
            bits = xEventGroupGetBits(gyroEventGroup);
            std::string calstep = "";
            display.setCursor(X_OFFSET, Y_MENU_OFFSET+30);
            display.print("Entering if block");
            if (bits & GYRO_EVT_CALIBRATE_COMPLETE){
                display.print("Breaking");
                break;
            } else {
                display.setCursor(X_OFFSET, Y_OFFSET);
                display.print("Going into calibration");
                if (xQueueReceive(calQueue, &calstep, 0) == pdPASS){
                    printf("%s", calstep.c_str());
                    display.print(calstep.c_str());
                }
            }
        }
        display.setTextSize(5);
        display.setCursor(120,120);
        display.print("FINISHED");
    }
    */

    // =======================================================================================

    
    

}
