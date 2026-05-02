#pragma once

namespace Earendil_Radio {
    typedef struct Radio_SharedData {
        double rx_latitude_rad;
        double rx_longitude_rad;
        
        double rx_latitude_deg;
        double rx_longitude_deg;
        
        float rx_sea_level_hpa;
    } Radio_SharedData_t;
}