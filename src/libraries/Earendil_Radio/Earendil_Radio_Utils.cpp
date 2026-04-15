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

    void sendPing_TX(
        uint16_t recipient_id,
        uint16_t sender_id,
        uint16_t message_type,
        uint16_t message_id,
        uint16_t message_att,
        uint32_t date_sent,
        uint32_t time_sent
    ){        
        std::vector<uint8_t> radio_packet;
        uint16_t recipient_id   = 1488;
        std::vector<uint8_t> metadata;
        uint16_t sender_id      = 1313;
        uint16_t message_type   = static_cast<uint16_t>(MessageType::PING_REQUEST);
        uint16_t message_id     = 0;
        uint16_t message_att    = 0;
        uint32_t date_sent      = 1234;
        uint32_t time_sent      = 5678;

        encodeMetadata(
            metadata, 
            sender_id, 
            message_type, 
            message_id, 
            message_attempt, 
            date_sent, 
            time_sent
        );
        encodePacket(
            radio_packet, 
            recipient_id, 
            metadata, 
            data
        );

        gpio_put(13, 1);
        if (radio.send(radio_packet.data(), radio_packet.size())) {
            radio.waitPacketSent();
        }
        gpio_put(13, 0);
    }

    void listen_RX(void){

    }

    void encodeDate(void){
        ;
    }

    void encodePacket(
        std::vector<uint8_t>&       radio_packet,
        uint16_t                    recipient_id
        const std::vector<uint8_t>& metadata,
        const std::vector<uint8_t>& data,
    ){
        radio_packet.push_back(static_cast<uint8_t>( (recipient_id >> 8 ) & 0xFF ));
        radio_packet.push_back(static_cast<uint8_t>( (recipient_id >> 0 ) & 0xFF ));

        for (size_t i = 0; i < metadata.size(); i++){
            radio_packet.push_back( metadata[i] );
        }

        for (size_t i = 0; i < data.size(); i++){
            radio_packet.push_back( data[i] );
        }
    }

    void encodeMetadata(
        std::vector<uint8_t>&   metadata,
        uint16_t                sender_id, 
        uint16_t                message_type,
        uint16_t                message_id,
        uint16_t                message_att,
        uint32_t                date_sent,
        uint32_t                time_sent
    ){  
        metadata.push_back(static_cast<uint8_t>( (sender_id >> 8 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (sender_id >> 0 ) & 0xFF ));

        metadata.push_back(static_cast<uint8_t>( (message_type >> 8 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (message_type >> 0 ) & 0xFF ));

        metadata.push_back(static_cast<uint8_t>( (message_id >> 8 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (message_id >> 0 ) & 0xFF ));

        metadata.push_back(static_cast<uint8_t>( (message_att >> 8 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (message_att >> 0 ) & 0xFF ));

        metadata.push_back(static_cast<uint8_t>( (date_sent >> 24 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (date_sent >> 16 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (date_sent >> 8  ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (date_sent >> 0  ) & 0xFF ));

        metadata.push_back(static_cast<uint8_t>( (time_sent >> 24 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (time_sent >> 16 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (time_sent >> 8  ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (time_sent >> 0  ) & 0xFF ));
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