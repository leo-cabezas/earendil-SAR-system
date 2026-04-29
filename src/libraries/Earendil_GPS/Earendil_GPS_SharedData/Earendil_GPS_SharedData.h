#pragma once

namespace Earendil_GPS {
    typedef struct GPS_SharedData {
        double      latitude_rad;
        double      longitude_rad;

        double      latitude_deg;  // Positive = N, Negative = S
        double      longitude_deg; // Positive = E, Negative = W
        
        uint16_t    year;
        uint8_t     month;
        uint8_t     day;

        uint8_t     hour;
        uint8_t     minute;
        uint8_t     second;
        // uint8_t     centisecond;
    } GPS_SharedData_t;
}