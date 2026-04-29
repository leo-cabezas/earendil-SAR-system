#pragma once

namespace Earendil_Altimeter {
    typedef struct Altimeter_SharedData {

        float temperature;
        float pressure;
        float altitude;
        float sea_level;

    } Altimeter_SharedData_t;
}