#include <Earendil_uSDReader.h> // ATTENTION: Add all library dependencies to this header.

namespace Earendil_uSDReader {

    SdExFat sd;
    ExFile file;

    void setup(){

        if (!sd.begin(USD_CS)) {
            while(1){
                //printf("SD Card Module failed initializaion!");
                vTaskDelay(pdMS_TO_TICKS(5000));
            };
        }
        // if (!file.open("logs.txt", O_WRONLY | O_CREAT | O_APPEND)) {
        //     while(1) {
        //         printf("SD Card: File open failed.");
        //         vTaskDelay(pdMS_TO_TICKS(5000));
        //     };
        //   }

        // vTaskDelay(pdMS_TO_TICKS(20000)); //This delay is to allow the GPS to get a fix. Not sure where the task is that calls the setup function for the SD card so not sure how to get the GPS to let it know when it first turns on.
        // //ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
        
        // char buf[64];
        // snprintf(buf, sizeof(buf), "System Start: %d:%02d:%02d %02d:%02d:%02d\n",
        //     Earendil_Data->GPS_Data.year,
        //     Earendil_Data->GPS_Data.month,
        //     Earendil_Data->GPS_Data.day,
        //     Earendil_Data->GPS_Data.hour,
        //     Earendil_Data->GPS_Data.minute,
        //     Earendil_Data->GPS_Data.second
        // );

        //file.print(buf);
        //file.close();
    }

    void logData(){
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (!file.open("logs.txt", O_WRONLY | O_CREAT | O_APPEND)) {
            while(1) {
                //printf("SD Card: File open failed.");
                vTaskDelay(pdMS_TO_TICKS(5000));
            };
          }

        char buf[64];
        snprintf(buf, sizeof(buf), "User packet received: %d:%02d:%02d %02d:%02d:%02d - \n",
            Earendil_Data->GPS_Data.year,
            Earendil_Data->GPS_Data.month,
            Earendil_Data->GPS_Data.day,
            Earendil_Data->GPS_Data.hour,
            Earendil_Data->GPS_Data.minute,
            Earendil_Data->GPS_Data.second
        );

        file.print(buf);
        file.close();
    }

}