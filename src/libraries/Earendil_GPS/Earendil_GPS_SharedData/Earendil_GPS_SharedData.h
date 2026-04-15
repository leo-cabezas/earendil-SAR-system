#pragma once

namespace Earendil_GPS {
    typedef struct GPS_SharedData {
        float       latitude_deg;  // Positive = N, Negative = S
        float       latitude_rad;
        float       longitude_deg; // Positive = E, Negative = W
        float       longitude_rad;

        uint16_t    year;
        uint8_t     month;
        uint8_t     day;

        uint8_t     hour;
        uint8_t     minute;
        uint8_t     second;
        // uint8_t     centisecond;
    } GPS_SharedData_t;
}