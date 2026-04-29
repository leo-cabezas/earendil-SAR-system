#include <Earendil_Display.h>   // ATTENTION: Add all library dependencies inside this header.

namespace Earendil_Display {
    
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
        createTask_vDisplay_UI();
        createTask_vDisplay_Controls();
    }

    // =======================================================================================
    // vDisplay_UI
    // =======================================================================================

    void createTask_vDisplay_UI(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_UI;
        xTaskCreate(
            vDisplay_UI,                                    // Task function
            "vDisplay_UI",                                  // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_UI(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        while(1){
            xSemaphoreTake(Earendil_Mutexes->spi_mutex, portMAX_DELAY);
            if (previous_ui != active_ui){
                switch (active_ui){
                    case NAVIGATION_UI:
                        draw_NavScreen_Static();
                        break;
                    case MENU_UI:
                        draw_MenuScreen_Static();
                        break;
                }
                previous_ui = active_ui;
            }
            switch (active_ui){
                case NAVIGATION_UI:
                    draw_NavScreen_Dynamic();
                    break;
                case MENU_UI:     
                    draw_MenuScreen_Dynamic();
                    break;
                case TESTING_UI:
                    drawTesting();
                    break;
                case CALIBRATING_UI:
                    display.fillScreen(GC9A01A_BLACK);
                    display.setCursor(40, 120);
                    display.print("Calibrating...");
                    break;
            }
            xSemaphoreGive(Earendil_Mutexes->spi_mutex);

            vTaskDelay(pdMS_TO_TICKS(250));
        }
    }

    // =======================================================================================
    // vDisplay_Controls
    // =======================================================================================

    void createTask_vDisplay_Controls(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->Display_Handles.task_vDisplay_Controls;
        xTaskCreate(
            vDisplay_Controls,                              // Task function
            "vDisplay_Controls",                            // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            2,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vDisplay_Controls(void* pvParameters){
        (void)pvParameters;     // Parameters unused.
        
        while(1){
            /*
            if (ulTaskNotifyTake(pdTRUE, 0) > 0)
            {
                if (active_ui == CALIBRATING_UI)
                {
                    active_ui = MENU_UI;
                    drawMenuScreen();
                }
            }
            */
            switch (active_ui){
                case NAVIGATION_UI:
                    controlNav();
                    break;
                case MENU_UI:
                    menuControl();
                    break;
                
                case TESTING_UI:
                    menuControl();
                    break;
                case CALIBRATING_UI:
                    break;
            }
            vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
    
    // =======================================================================================
    // vDisplay_Calibration
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
