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
            printf("Temperature: %.2f *C\nPressure: %.2f hPa\nCalculated Altitude: %.2f m\n", temperature, pressure, altitude);
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

void updateDistanceHeading(float& distance, float& heading){    // Is float enough?
    float handheld_latitude_rad = GPS_Handheld.latitudeDegrees;
    float handheld_longitude_rad = GPS_Handheld.longitudeDegrees;
    // float handheld_altitude = GPS_Handheld.;

    float node_latitude_rad = GPS_Node.latitudeDegrees;
    float node_longitude_rad = GPS_Node.longitudeDegrees;
    // float node_altitude = GPS_Node.

    /*
    float handheld_X = (EARTH_RADIUS + handheld_altitude) * sin(handheld_longitude) * cos(handheld_latitude - 90.0);
    float handheld_Y = (EARTH_RADIUS + handheld_altitude) * sin(handheld_longitude) * sin(handheld_latitude - 90.0);
    float handheld_Z = handheld_altitude;

    float node_X = (EARTH_RADIUS + node_altitude) * sin(node_longitude) * cos(node_latitude - 90.0);
    float node_Y = (EARTH_RADIUS + node_altitude) * sin(node_longitude) * sin(node_latitude - 90.0);
    float node_Z = node_altitude;
    
    float delta_X = node_X - handheld_X;
    float delta_Y = node_Y - handheld_Y;
    float delta_Z = node_Z - handheld_Z;

    distance = sqrt(delta_X * delta_X + delta_Y * delta_Y + delta_Z * delta_Z);
    heading = Still need to think about this one.
    */
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

