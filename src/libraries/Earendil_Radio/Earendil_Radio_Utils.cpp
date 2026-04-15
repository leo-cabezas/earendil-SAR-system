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
        radio.setTxPower(23, false); // Power in dBm.

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
        std::vector<uint8_t> metadata;
        std::vector<uint8_t> data = {13, 14, 15, 16, 17, 18};

        encodeMetadata(
            metadata,
            recipient_id,
            sender_id, 
            message_type, 
            message_id, 
            message_att, 
            date_sent, 
            time_sent
        );

        encodePacket(
            radio_packet, 
            metadata, 
            data
        );

        gpio_put(13, 1);
        if (radio.send(radio_packet.data(), radio_packet.size())) {
            radio.waitPacketSent();
        }
        gpio_put(13, 0);
    }

    void listen_RX(){
        if (radio.available()){
            uint8_t radio_rx_buffer[RH_RF95_MAX_MESSAGE_LEN];
            uint8_t buffer_len = sizeof(radio_rx_buffer);

            if (radio.recv(radio_rx_buffer, &buffer_len)){
                // int rssi = radio.lastRssi()
                std::vector<uint8_t> radio_packet = std::vector<uint8_t>(radio_rx_buffer, radio_rx_buffer + buffer_len);
                std::vector<uint8_t> metadata;
                std::vector<uint8_t> data;
                
                uint16_t recipient_id;
                uint16_t sender_id;
                uint16_t message_type;
                uint16_t message_id;
                uint16_t message_att;
                uint32_t date_sent;
                uint32_t time_sent;

                decodePacket(
                    radio_packet,
                    metadata,
                    data
                );

                decodeMetadata(
                    metadata,
                    recipient_id,
                    sender_id, 
                    message_type, 
                    message_id, 
                    message_att, 
                    date_sent, 
                    time_sent
                );
            }
        }
    }

    void encodePacket(
        std::vector<uint8_t>&       radio_packet,
        const std::vector<uint8_t>& metadata,
        const std::vector<uint8_t>& data
    ){
        for (size_t i = 0; i < metadata.size(); i++){
            radio_packet.push_back(metadata[i]);
        }

        for (size_t i = 0; i < data.size(); i++){
            radio_packet.push_back(data[i]);
        }
    }

    void decodePacket(
        const std::vector<uint8_t>& radio_packet,
        std::vector<uint8_t>&       metadata,
        std::vector<uint8_t>&       data
    ){  
        constexpr size_t METADATA_SIZE_BYTES    = 18;
        constexpr size_t DATA_SIZE_BYTES        = 6;    // MAY BE DIFFERENT FOR DIFFERENT DATA
        size_t byte = 0;

        metadata.clear();
        metadata.reserve(METADATA_SIZE_BYTES);
        for (size_t i = 0; (i < METADATA_SIZE_BYTES) && (byte < radio_packet.size()); i++){
            metadata.push_back(radio_packet[byte++]);
        }

        data.clear();
        data.reserve(DATA_SIZE_BYTES);
        for (size_t i = 0; (i < DATA_SIZE_BYTES) && (byte < radio_packet.size()); i++){
            data.push_back(radio_packet[byte++]);
        }
    }

    void encodeMetadata(
        std::vector<uint8_t>&   metadata,
        uint16_t                recipient_id,
        uint16_t                sender_id, 
        uint16_t                message_type,
        uint16_t                message_id,
        uint16_t                message_att,
        uint32_t                date_sent,
        uint32_t                time_sent
    ){  
        metadata.push_back(static_cast<uint8_t>( (recipient_id >> 8 ) & 0xFF ));
        metadata.push_back(static_cast<uint8_t>( (recipient_id >> 0 ) & 0xFF ));

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

    void decodeMetadata(
        const std::vector<uint8_t>& metadata,
        uint16_t&                   recipient_id, 
        uint16_t&                   sender_id, 
        uint16_t&                   message_type,
        uint16_t&                   message_id,
        uint16_t&                   message_att,
        uint32_t&                   date_sent,
        uint32_t&                   time_sent
    ){
        size_t byte = 0;

        recipient_id = 0;
        recipient_id |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        recipient_id |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        sender_id = 0;
        sender_id |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        sender_id |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        message_type = 0;
        message_type |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        message_type |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        message_id = 0;
        message_id |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        message_id |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        message_att = 0;
        message_att |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        message_att |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        date_sent = 0;
        date_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 24;
        date_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 16;
        date_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        date_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 0;

        time_sent = 0;
        time_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 24;
        time_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 16;
        time_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 8;
        time_sent |= (static_cast<uint16_t>( metadata[byte++] )) << 0;
    }

    void encodeData(
        const std::vector<uint8_t>& data
        uint16_t message_type
    ){
        /*
        switch (message_type){
            case 
        }
        */
    }

    void decodeData(
        const std::vector<uint8_t>& data
        uint16_t message_type
    ){
        /*
        switch (message_type){
            case 
        }
        */
    }

    double getLatitude(uint8_t raw_latitude[5]){    // TEMPORARY. Used to decode radio-received
        uint8_t degrees = raw_latitude[0];
        double minutes =

        return  ((double)(buf[1]) + (double)(buf[2]) / 100 + (double)(buf[3]) / 10000) / 60.0) * ;
    }

    double getLongitude(uint8_t raw_longitude[6]){  // TEMPORARY. Used to decode radio-received GPS info.
        return ((double)(raw_longitude[0]) * 100 + (double)(raw_longitude[0]) + ((double)(buf[2]) + (double)(buf[3]) / 100 + (double)(buf[4]) / 10000) / 60.0) * (buf[5] == 'E' ? 1 : -1);
    }
}

/*
static inline void encodeGPSData(){

}

static inline void decodeGPSData(){

}


static inline 



static inline double getAltitude(uint8_t raw_altitude[]){
  // Not implemented yet.
  return;
}

*/