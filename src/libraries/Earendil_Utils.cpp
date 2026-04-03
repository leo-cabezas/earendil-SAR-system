#include <Earendil_Utils.h>

//GPS Data structs for use in the final distance calculations. Specific tasks can write to them then the calculator function can read from them.
GPSData_t GPS_Handheld;
GPSData_t GPS_Node;

//Hypsometric equation function used to calculate the altitude based on temperature in Kelvin and pressure in hPa
static inline float hypsometricAltitude(float temp, float pressure){
    float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
    return altitude;
}

// //Hypsometric equation function used to calculate the local sea level pressure based on temperature in Kelvin and pressure in hPa
// static inline float hypsometric_pressure(float temp, float pressure){
//   float sealevelpressure = (pressure) / (pow(1 - (LAPSE_RATE * LOCAL_ALTITUDE / temp), HYPSOMETRIC_CONSTANT));
//   return sealevelpressure;
// }

void vAltitudeUtility(void* pvParameters){
    SensorData_t bench;
    float temperature = 0;
    float pressure = 0;
    float altitude = 0;

    while(1){
        if (xQueueReceive(auQueue, &bench, portMAX_DELAY) == pdPASS) {
            if (bench.eSource == altTemp){
                temperature = bench.eData;
                // printf("Received temperature: %f *C\n", temperature);
            }
            if (bench.eSource == altPress){
                pressure = bench.eData / 100;
                // printf("Received pressure: %f hPa\n", pressure);
            }
            altitude = hypsometricAltitude(temperature + 273.15, pressure);
            printf("Temperature: %f *C\nPressure: %f hPa\nCalculated Altitude: %f m\n", temperature, pressure, altitude);
        }
    }
}

// void vGetPressure(void* pvParameters);

// void destinationPoint(float lat1, float lon1, float bearing_deg, float distance_m, float *lat2, float *lon2) {
//     float lat1r  = lat1 * M_PI / 180.0f;
//     float lon1r  = lon1 * M_PI / 180.0f;
//     float brng   = bearing_deg * M_PI / 180.0f;
//     float dR     = distance_m / EARTH_RADIUS;

//     *lat2 = asin(sin(lat1r) * cos(dR) + cos(lat1r) * sin(dR) * cos(brng));

//     *lon2 = lon1r + atan2(sin(brng) * sin(dR) * cos(lat1r), cos(dR) - sin(lat1r) * sin(*lat2));

//     *lat2 = *lat2 * 180.0f / M_PI;
//     *lon2 = *lon2 * 180.0f / M_PI;
// }

float getDistance(){
    float hh_lat = GPS_Handheld.latitudeDegrees;
    float hh_long = GPS_Handheld.longitudeDegrees;
    float nd_lat = GPS_Node.latitudeDegrees;
    float nd_long = GPS_Node.longitudeDegrees;
    //math
    float distance = 0; //meters
    float direction = 0; //degrees
    return distance;
}

void vGPSRXUtility(void* pvParameters){
    GPSData_t gpsBench;
    // float lat2, lon2;

    while(1){
        if (xQueueReceive(guQueue, &gpsBench, portMAX_DELAY) == pdPASS) {
            if (gpsBench.systemSource == eHandheld){
                printf("GPS Data Received from Handheld.\n%02u/%02u/%02u %02u:%02u:%02u UTC\nLatitude: %f, Longitude: %f\n",
                    gpsBench.day,
                    gpsBench.month,
                    gpsBench.year,
                    gpsBench.hour,
                    gpsBench.minute,
                    gpsBench.seconds,
                    gpsBench.latitudeDegrees,
                    gpsBench.longitudeDegrees
                );
                GPS_Handheld = gpsBench;
            }
            if (gpsBench.systemSource == eNode){
                printf("GPS Data Received from Node.\n%02u/%02u/%02u %02u:%02u:%02u UTC\nLatitude: %f, Longitude: %f\n",
                    gpsBench.day,
                    gpsBench.month,
                    gpsBench.year,
                    gpsBench.hour,
                    gpsBench.minute,
                    gpsBench.seconds,
                    gpsBench.latitudeDegrees,
                    gpsBench.longitudeDegrees
                );
                GPS_Node = gpsBench;
            }
        }
        vTaskDelay(pdMS_TO_TICKS(5000));
        // destinationPoint(38.971669, -95.235060, 90.0f, 500.0f, &lat2, &lon2);
        // printf("500m East: %.6f, %.6f\n", lat2, lon2);
    }
}

