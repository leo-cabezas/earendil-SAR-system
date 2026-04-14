#include <Earendil_Radio.h>

namespace Earendil_Radio {

    RH_RF95 radio = RH_RF95(RFM95_CS_PIN, RFM95_IRQ_PIN);

    void setup(){
        setupRadio();
    }

    void setupRadio(void){
        // Reset the LoRa module
        pinMode(RFM95_RST_PIN, OUTPUT);
        digitalWrite(RFM95_RST_PIN, LOW);
        digitalWrite(RFM95_RST_PIN, HIGH);

        // Initialize RF95
        radio.init();
        radio.setFrequency(915.0);
        // Should also set dBm!

        gpio_init(13);
        gpio_set_dir(13, GPIO_OUT);
    }

    void sendPing_TX(void){

        uint8_t buf[64];
        buf[0] = (uint8_t) 12;
        buf[1] = (uint8_t) 34;
        buf[2] = (uint8_t) 56;
        buf[3] = (uint8_t) 78;
        buf[4] = (uint8_t) 'S';
        int len = 5;
        
        gpio_put(13, 1);

        // Send packet
        vTaskSuspend(Earendil_Handles->Display_Handles.task_vDisplay_MenuScreen);
        vTaskSuspend(Earendil_Handles->Display_Handles.task_vDisplay_MenuControl);
        if (radio.send((uint8_t*)buf, len + 1)) {
            radio.waitPacketSent();
        }
        vTaskResume(Earendil_Handles->Display_Handles.task_vDisplay_MenuScreen);
        vTaskResume(Earendil_Handles->Display_Handles.task_vDisplay_MenuControl);
        gpio_put(13, 0);
    }

    // 
    // ATTENTION: MIGHT BE A GOOD IDEA TO UPDATE THIS CODE TO USE C++ VECTORS. MORE MEMORY SAFE, WORTH.
    //
    #define METADATA_FIELD_COUNT 4
    void encodePacket(    // TESTED, NEED TO INCLUDE ACTUAL SENT DATA.
        uint8_t*    packet,
        uint32_t    recipient_id,
        uint8_t     (&metadata)[METADATA_FIELD_COUNT * 4]
        // add data here. Probably a good idea to use std::vector<uint8_t>
    ){
        const uint8_t recipient_id_index = 0;
        packet[recipient_id_index + 0] = (recipient_id >> 24) & 0xFF;
        packet[recipient_id_index + 1] = (recipient_id >> 16) & 0xFF;
        packet[recipient_id_index + 2] = (recipient_id >> 8)  & 0xFF;
        packet[recipient_id_index + 3] = (recipient_id >> 0)  & 0xFF; 

        const uint8_t metadata_index = 4;
        for (size_t i = 0; i < METADATA_FIELD_COUNT * 4; i++){
            packet[metadata_index + i] = metadata[i];
        }
    }

    void encodeMetadata(  // TESTED AND FUNCTIONAL.
        uint8_t     (&metadata)[METADATA_FIELD_COUNT * 4],
        uint32_t    sender_id, 
        uint32_t    message_id,
        uint32_t    message_attempt,
        uint32_t    timestamp
    ){  
        uint32_t metadata_fields[METADATA_FIELD_COUNT] = {sender_id, message_id, message_attempt, timestamp};

        uint8_t field_index = 0;
        for (size_t i = 0; i < METADATA_FIELD_COUNT; i++){
            metadata[field_index + 0] = (metadata_fields[i] >> 24) & 0xFF;
            metadata[field_index + 1] = (metadata_fields[i] >> 16) & 0xFF;
            metadata[field_index + 2] = (metadata_fields[i] >> 8)  & 0xFF;
            metadata[field_index + 3] = (metadata_fields[i] >> 0)  & 0xFF;
            field_index += 4;
        }
    }

    void decodeMetadata(  // TESTED AND FUNCTIONAL.
        uint8_t     metadata[METADATA_FIELD_COUNT * 4],
        uint32_t&   sender_id, 
        uint32_t&   message_id,
        uint32_t&   message_attempt,
        uint32_t&   timestamp
    ){
        uint32_t* metadata_fields[METADATA_FIELD_COUNT] = {&sender_id, &message_id, &message_attempt, &timestamp};

        uint8_t field_index = 0;
        for (size_t i = 0; i < METADATA_FIELD_COUNT; i++){
            uint32_t retrieved_value = 0;
            retrieved_value |= metadata[field_index + 0] << 24;
            retrieved_value |= metadata[field_index + 1] << 16;
            retrieved_value |= metadata[field_index + 2] << 8;
            retrieved_value |= metadata[field_index + 3] << 0;
            *metadata_fields[i] = retrieved_value;
            field_index += 4;
        }
    }

}

/*
static inline void encodeGPSData(){

}

static inline void decodeGPSData(){

}


static inline double getLatitude(uint8_t raw_latitude[5]){    // TEMPORARY. Used to decode radio-received
  uint8_t degrees = raw_latitude[0];
  double minutes =

  return  ((double)(buf[1]) + (double)(buf[2]) / 100 + (double)(buf[3]) / 10000) / 60.0) * ;
}

static inline double getLongitude(uint8_t raw_longitude[6]){  // TEMPORARY. Used to decode radio-received GPS info.
  return ((double)(raw_longitude[0]) * 100 + (double)(raw_longitude[0]) + ((double)(buf[2]) + (double)(buf[3]) / 100 + (double)(buf[4]) / 10000) / 60.0) * (buf[5] == 'E' ? 1 : -1);
}

static inline double getAltitude(uint8_t raw_altitude[]){
  // Not implemented yet.
  return;
}

*/