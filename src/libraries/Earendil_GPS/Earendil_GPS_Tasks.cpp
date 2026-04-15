#include <Earendil_GPS.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_GPS {

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

    }

    // =======================================================================================
    // vGPS_
    // =======================================================================================

    void createTask_vGPS_(void){
        TaskHandle_t* task_handle_ptr = &Earendil_Handles->GPS_Handles.task_vDisplay_MenuScreen;
        xTaskCreate(
            vGPS_,                                          // Task function
            "vGPS_",                                        // Task name (for debugging)
            512,                                            // Task stack depth (in words, NOT bytes!)
            NULL,                                           // Task parameters at creation
            1,                                              // Real-time task priority
            task_handle_ptr                                 // Task handle
        );
        vTaskCoreAffinitySet(*task_handle_ptr, 1 << 
            0                                               // Assigned core (options: 0, 1)
        );
    }

    void vGPS_(void* pvParameters){
        (void)pvParameters;     // Parameters unused.

        vTaskSuspend(NULL); // Initially self-suspend.
        
        while(1){
            
            // vTaskDelay(pdMS_TO_TICKS(50));
        }
    }
}

/*
GPSData_t gpsBench;

void vGPS(void* pvParameters){
  (void)pvParameters;

  vTaskDelay(pdMS_TO_TICKS(5000));
  
  TickType_t lastNotify = xTaskGetTickCount();
  // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
  GPS.begin(9600); //printf("Line 14 OK\n");
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); printf("Line 18 OK\n");
  // uncomment this line to turn on only the "minimum recommended" data
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); //printf("Line 18 OK\n");
  // Set the update rate
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //printf("Line 20 OK\n");
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  // Request updates on antenna status, comment out to keep quiet
  // GPS.sendCommand(PGCMD_ANTENNA); printf("Line 24 OK\n");
  vTaskDelay(pdMS_TO_TICKS(1000)); //printf("Line 25 OK\n");
  
  while (1){ // printf("Inside task loop!\n");
    // read data from the GPS in the 'main loop'
    char c = GPS.read();
    // if you want to debug, this is a good time to do it!
    // if (GPSECHO)
    //   if (c) Serial.print(c);
    // if a sentence is received, we can check the checksum, parse it...
    if (GPS.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      //Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        printf("FAILED TO PARSE!\n"); // we can fail to parse a sentence in which case we should just wait for another
    }

    // approximately every 10 seconds, put all of the last sentence data into the shared GPS struct
    if ((xTaskGetTickCount() - lastNotify) >= pdMS_TO_TICKS(10000)) {
      xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
          gpsBench.latitudeDegrees  = GPS.latitudeDegrees;
          gpsBench.longitudeDegrees = GPS.longitudeDegrees;
          gpsBench.year             = GPS.year;
          gpsBench.month            = GPS.month;
          gpsBench.day              = GPS.day;
          gpsBench.hour             = GPS.hour;
          gpsBench.minute           = GPS.minute;
          gpsBench.seconds          = GPS.seconds;
          // gpsBench.fix              = GPS.fix;
      xSemaphoreGive(gpsDataMutex);

      if (GPS.fix){
        // printf("DEBUG: %02u:%02u:%02u\n", GPS.hour, GPS.minute, GPS.seconds);
        xTaskNotifyGive(taskGPSTX);
      }

      lastNotify = xTaskGetTickCount();
    }
  }
}

void vGPSTX(void* pvParameters){

  vTaskDelay(pdMS_TO_TICKS(7000));

  GPSData_t snapshot;
  snapshot.systemSource = eHandheld;

  while(1){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // sleep until notified
        xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
            snapshot = gpsBench;
        xSemaphoreGive(gpsDataMutex);
        // printf("Lat: %f Lon: %f\n", snapshot.latitudeDegrees, snapshot.longitudeDegrees);
        xQueueSend(guQueue, &snapshot, 0);
  }
}

*/