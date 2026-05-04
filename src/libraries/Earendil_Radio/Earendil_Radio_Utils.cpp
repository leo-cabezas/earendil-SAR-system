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
        if (!radio.init()){
            while (1){
                gpio_put(LED_BUILTIN, 1);
                //vTaskDelay(pdMS_TO_TICKS(10));
                sleep_ms(10);
                gpio_put(LED_BUILTIN, 0);
                //vTaskDelay(pdMS_TO_TICKS(10));
                sleep_ms(10);
            }
        }

        radio.setFrequency(915.0);
        radio.setTxPower(23, false); // Power in dBm.
        radio.setModeRx();

        gpio_init(LED_BUILTIN);
        gpio_set_dir(LED_BUILTIN, GPIO_OUT);

        Earendil_Data->Radio_Data.rx_sea_level_hpa = -1;
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
        std::vector<uint8_t> data;

        switch (message_type){
            case 1: {
                // Mutex here if necessary
                double latitude_rad     = Earendil_Data->GPS_Data.latitude_rad;
                double longitude_rad    = Earendil_Data->GPS_Data.longitude_rad;
                // Mutex here if necessary

                encodeGPSData(
                    data,
                    latitude_rad,
                    longitude_rad
                );
                break;
            }
            case 2: {
                // Mutex here if necessary
                float sea_level         = Earendil_Data->Altimeter_Data.sea_level;
                // Mutex here if necessary

                encodeAltimeterData(
                    data,
                    sea_level
                );
                break;
            }
        }

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

        gpio_put(LED_BUILTIN, 1);

        xSemaphoreTake(Earendil_Mutexes->spi_mutex, portMAX_DELAY);
        // radio.setModeTx();
        if (radio.send(radio_packet.data(), radio_packet.size())) {
            radio.waitPacketSent();
        }
        // radio.setModeRx();
        xSemaphoreGive(Earendil_Mutexes->spi_mutex);

        gpio_put(LED_BUILTIN, 0);
    }

    void listen_RX(){
        uint8_t radio_rx_buffer[RH_RF95_MAX_MESSAGE_LEN];
        uint8_t buffer_len = sizeof(radio_rx_buffer);
        bool radio_recv = false;

        xSemaphoreTake(Earendil_Mutexes->spi_mutex, portMAX_DELAY);
        if (radio.available()){
            radio_recv = radio.recv(radio_rx_buffer, &buffer_len);
        }   
        xSemaphoreGive(Earendil_Mutexes->spi_mutex);

        if (!radio_recv){
            return;
        }

        gpio_xor_mask(1u << LED_BUILTIN);

        // int rssi = radio.lastRssi()

        std::vector<uint8_t> radio_packet = std::vector<uint8_t>(radio_rx_buffer, radio_rx_buffer + buffer_len);
        std::vector<uint8_t> metadata;
        std::vector<uint8_t> data;
        
        decodePacket(
            radio_packet,
            metadata,
            data
        );
        
        uint16_t recipient_id;
        uint16_t sender_id;
        uint16_t message_type;
        uint16_t message_id;
        uint16_t message_att;
        uint32_t date_sent;
        uint32_t time_sent;

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
        
        char debug_buf[64];

        switch (message_type){
            case 1: {
                double rx_latitude_rad;
                double rx_longitude_rad;

                decodeGPSData(
                    data,
                    rx_latitude_rad,
                    rx_longitude_rad
                );

                Earendil_Data->Radio_Data.rx_latitude_rad   = rx_latitude_rad;
                Earendil_Data->Radio_Data.rx_longitude_rad  = rx_longitude_rad;
                
                Earendil_Data->Radio_Data.rx_latitude_deg   = rx_latitude_rad   * (180.0 / M_PI);
                Earendil_Data->Radio_Data.rx_longitude_deg  = rx_longitude_rad  * (180.0 / M_PI);

                snprintf(debug_buf, sizeof(debug_buf), "Latitude: %f *\n", Earendil_Data->Radio_Data.rx_latitude_deg);
                puts(debug_buf);
                snprintf(debug_buf, sizeof(debug_buf), "Longitude: %f *\n", Earendil_Data->Radio_Data.rx_longitude_deg);
                puts(debug_buf);
                break;
            }
            case 2: {
                float  rx_sea_level;

                decodeAltimeterData(
                    data,
                    rx_sea_level
                );

                Earendil_Data->Radio_Data.rx_sea_level_hpa  = rx_sea_level;

                snprintf(debug_buf, sizeof(debug_buf), "Sea-Level Pressure: %f *\n", Earendil_Data->Radio_Data.rx_sea_level_hpa);
                puts(debug_buf);
                break;
            }
        }
    }

    void encodePacket(
        std::vector<uint8_t>&       radio_packet,
        const std::vector<uint8_t>& metadata,
        const std::vector<uint8_t>& data
    ){
        radio_packet.push_back(metadata.size());

        for (size_t i = 0; i < metadata.size(); i++){
            radio_packet.push_back(metadata[i]);
        }

        radio_packet.push_back(data.size());

        for (size_t i = 0; i < data.size(); i++){
            radio_packet.push_back(data[i]);
        }
    }

    void decodePacket(
        const std::vector<uint8_t>& radio_packet,
        std::vector<uint8_t>&       metadata,
        std::vector<uint8_t>&       data
    ){  
        size_t byte = 0;

        uint8_t metadata_size_bytes = radio_packet[byte++];

        metadata.clear();
        metadata.reserve(metadata_size_bytes);
        for (size_t i = 0; (i < metadata_size_bytes) && (byte < radio_packet.size()); i++){
            metadata.push_back(radio_packet[byte++]);
        }

        uint8_t data_size_bytes = radio_packet[byte++];

        data.clear();
        data.reserve(data_size_bytes);
        for (size_t i = 0; (i < data_size_bytes) && (byte < radio_packet.size()); i++){
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
        date_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 24;
        date_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 16;
        date_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 8;
        date_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 0;

        time_sent = 0;
        time_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 24;
        time_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 16;
        time_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 8;
        time_sent |= (static_cast<uint32_t>( metadata[byte++] )) << 0;
    }

    void encodeGPSData(
        std::vector<uint8_t>&   data,
        double                  latitude_rad,
        double                  longitude_rad
    ){
        int32_t int_latitude_rad = static_cast<int32_t>(latitude_rad * 1000000.0);
        data.push_back(static_cast<uint8_t>( (int_latitude_rad >> 24 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_latitude_rad >> 16 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_latitude_rad >> 8  ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_latitude_rad >> 0  ) & 0xFF )); 

        int32_t int_longitude_rad = static_cast<int32_t>(longitude_rad * 1000000.0);
        data.push_back(static_cast<uint8_t>( (int_longitude_rad >> 24 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_longitude_rad >> 16 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_longitude_rad >> 8  ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_longitude_rad >> 0  ) & 0xFF ));
    }

    void decodeGPSData(
        const std::vector<uint8_t>& data,
        double&                     latitude_rad,
        double&                     longitude_rad
    ){
        size_t byte = 0;
            
        int32_t int_latitude_rad = 0;
        int_latitude_rad |= (static_cast<int32_t>( data[byte++] )) << 24;
        int_latitude_rad |= (static_cast<int32_t>( data[byte++] )) << 16;
        int_latitude_rad |= (static_cast<int32_t>( data[byte++] )) << 8;
        int_latitude_rad |= (static_cast<int32_t>( data[byte++] )) << 0;
        latitude_rad = int_latitude_rad / 1000000.0;

        int32_t int_longitude_rad = 0;
        int_longitude_rad |= (static_cast<int32_t>( data[byte++] )) << 24;
        int_longitude_rad |= (static_cast<int32_t>( data[byte++] )) << 16;
        int_longitude_rad |= (static_cast<int32_t>( data[byte++] )) << 8;
        int_longitude_rad |= (static_cast<int32_t>( data[byte++] )) << 0;
        longitude_rad = int_longitude_rad / 1000000.0;
    }

    void encodeAltimeterData(
        std::vector<uint8_t>&   data,
        float                   sea_level
    ){
        
        int32_t int_sea_level = static_cast<int32_t>(sea_level * 1000000.0);
        data.push_back(static_cast<uint8_t>( (int_sea_level >> 24 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_sea_level >> 16 ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_sea_level >> 8  ) & 0xFF ));
        data.push_back(static_cast<uint8_t>( (int_sea_level >> 0  ) & 0xFF ));
    }

    void decodeAltimeterData(
        const std::vector<uint8_t>& data,
        float&                      sea_level
    ){
        size_t byte = 0;
        
        int32_t int_sea_level = 0;
        int_sea_level |= (static_cast<int32_t>( data[byte++] )) << 24;
        int_sea_level |= (static_cast<int32_t>( data[byte++] )) << 16;
        int_sea_level |= (static_cast<int32_t>( data[byte++] )) << 8;
        int_sea_level |= (static_cast<int32_t>( data[byte++] )) << 0;
        sea_level = int_sea_level / 1000000.0;
    }

}