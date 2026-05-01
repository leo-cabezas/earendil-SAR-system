#include <Earendil_GPS.h>   // ATTENTION: Add all library dependencies to this header.

namespace Earendil_GPS {

    Adafruit_GPS gps = Adafruit_GPS(&GPSSerial);

    void setup(){
        setupGPS();
    }

    void setupGPS(){
        // 9600 NMEA is the default baud rate for Adafruit MTK GPS's- some use 4800
        gps.begin(9600);
        // uncomment this line to turn on only the "minimum recommended" data
        gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
        // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
        //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
        // Sets the update rate
        // For the parsing code to work nicely and have time to sort thru the data, and
        // print it out we (Adafruit) don't suggest using anything higher than 1 Hz
        gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
        // Request updates on antenna status, comment out to keep quiet
        // GPS.sendCommand(PGCMD_ANTENNA);
    }

    void gpsUpdate(){
        while (gps.available()){
            gps.read();
        }
            
        if (gps.newNMEAreceived()){
            if (!gps.parse(gps.lastNMEA())){
                return;
            }
            
            if (gps.fix){
                // xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
                Earendil_Data->GPS_Data.latitude_rad    = gps.latitudeDegrees   * (M_PI / 180.0);
                Earendil_Data->GPS_Data.longitude_rad   = gps.longitudeDegrees  * (M_PI / 180.0);

                Earendil_Data->GPS_Data.latitude_deg    = gps.latitudeDegrees;
                Earendil_Data->GPS_Data.longitude_deg   = gps.longitudeDegrees;

                Earendil_Data->GPS_Data.year             = gps.year;
                Earendil_Data->GPS_Data.month            = gps.month;
                Earendil_Data->GPS_Data.day              = gps.day;
                Earendil_Data->GPS_Data.hour             = gps.hour;
                Earendil_Data->GPS_Data.minute           = gps.minute;
                Earendil_Data->GPS_Data.second           = gps.seconds;
                //xSemaphoreGive(gpsDataMutex);
            }
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
  gps.begin(9600); //printf("Line 14 OK\n");
  // uncomment this line to turn on RMC (recommended minimum) and GGA (fix data) including altitude
  //GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA); printf("Line 18 OK\n");
  // uncomment this line to turn on only the "minimum recommended" data
  gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY); //printf("Line 18 OK\n");
  // Set the update rate
  gps.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); //printf("Line 20 OK\n");
  // For the parsing code to work nicely and have time to sort thru the data, and
  // print it out we don't suggest using anything higher than 1 Hz
  // Request updates on antenna status, comment out to keep quiet
  // GPS.sendCommand(PGCMD_ANTENNA); printf("Line 24 OK\n");
  vTaskDelay(pdMS_TO_TICKS(1000)); //printf("Line 25 OK\n");
  
  while (1){ // printf("Inside task loop!\n");
    // read data from the GPS in the 'main loop'
    char c = gps.read();
    // if you want to debug, this is a good time to do it!
    // if (GPSECHO)
    //   if (c) Serial.print(c);
    // if a sentence is received, we can check the checksum, parse it...
    if (gps.newNMEAreceived()) {
      // a tricky thing here is if we print the NMEA sentence, or data
      // we end up not listening and catching other sentences!
      // so be very wary if using OUTPUT_ALLDATA and trying to print out data
      //Serial.print(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
      if (!gps.parse(gps.lastNMEA())) // this also sets the newNMEAreceived() flag to false
        printf("FAILED TO PARSE!\n"); // we can fail to parse a sentence in which case we should just wait for another
    }

    // approximately every 10 seconds, put all of the last sentence data into the shared GPS struct
    if ((xTaskGetTickCount() - lastNotify) >= pdMS_TO_TICKS(10000)) {
      xSemaphoreTake(gpsDataMutex, portMAX_DELAY);
          gpsBench.latitudeDegrees  = gps.latitudeDegrees;
          gpsBench.longitudeDegrees = gps.longitudeDegrees;
          gpsBench.year             = gps.year;
          gpsBench.month            = gps.month;
          gpsBench.day              = gps.day;
          gpsBench.hour             = gps.hour;
          gpsBench.minute           = gps.minute;
          gpsBench.seconds          = gps.seconds;
          // gpsBench.fix              = gps.fix;
      xSemaphoreGive(gpsDataMutex);

      if (gps.fix){
        // printf("DEBUG: %02u:%02u:%02u\n", gps.hour, gps.minute, gps.seconds);
        xTaskNotifyGive(taskGPSTX);
      }

      lastNotify = xTaskGetTickCount();
    }
  }
}
*/

/*
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