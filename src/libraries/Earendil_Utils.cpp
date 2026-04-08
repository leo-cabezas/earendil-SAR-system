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
            printf("\n============ALTIMETER READING============\nTemperature: %.2f *C\nPressure: %.2f hPa\nCalculated Altitude: %.2f m\n=========================================\n", temperature, pressure, altitude);
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

//     *lon2 = lon1r + v atan2(sin(brng) * sin(dR) * cos(lat1r), cos(dR) - sin(lat1r) * sin(*lat2));

//     *lat2 = *lat2 * 180.0f / M_PI;
//     *lon2 = *lon2 * 180.0f / M_PI;
// }

void updateDistance(float& distance){    // Is float enough precision?
    //float handheld_latitude_rad = GPS_Handheld.latitudeRadians;
    //float handheld_longitude_rad = GPS_Handheld.longitudeRadians;
    // float handheld_altitude = GPS_Handheld.;

    //float node_latitude_rad = GPS_Node.latitudeRadians;
    //float node_longitude_rad = GPS_Node.longitudeRadians;
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
    */
}

void getBearingToNode(double& bearing_to_node_deg){
    // Need to take into account latitude difference w.r.t. spherical coordinates.
    double handheld_latitude_rad = 0.6;
    double handheld_longitude_rad = 6.2;
    double node_latitude_rad = 0.9;
    double node_longitude_rad = 0.2;

    double handheld_X = EARTH_RADIUS * sin(handheld_latitude_rad) * cos(handheld_longitude_rad);
    double handheld_Y = EARTH_RADIUS * sin(handheld_latitude_rad) * sin(handheld_longitude_rad);
    double handheld_Z = EARTH_RADIUS * cos(handheld_latitude_rad);
    double node_X = EARTH_RADIUS * sin(node_latitude_rad) * cos(node_longitude_rad);
    double node_Y = EARTH_RADIUS * sin(node_latitude_rad) * sin(node_longitude_rad);
    double node_Z = EARTH_RADIUS * cos(node_latitude_rad);
    
    double Hx_sqr = handheld_X * handheld_X;
    double Hy_sqr = handheld_Y * handheld_Y;
    double Hz_sqr = handheld_Z * handheld_Z;
    double HxNx = handheld_X * node_X;
    double HyNy = handheld_Y * node_Y;
    double HzNz = handheld_Z * node_Z;
    
    double mag_north_vec_X = (-1) * EARTH_RADIUS * handheld_X * handheld_Z;
    double mag_north_vec_Y = (-1) * EARTH_RADIUS * handheld_Y * handheld_Z;
    double mag_north_vec_Z = EARTH_RADIUS * (Hx_sqr + Hy_sqr);
    double mag_north_vec_mag = sqrt(pow(mag_north_vec_X, 2) + pow(mag_north_vec_Y, 2) + pow(mag_north_vec_Z, 2));

    double bearing_vec_X = node_X * (Hy_sqr + Hz_sqr) - handheld_X * (HyNy + HzNz); 
    double bearing_vec_Y = node_Y * (Hx_sqr + Hz_sqr) - handheld_Y * (HxNx + HzNz);
    double bearing_vec_Z = node_Z * (Hx_sqr + Hy_sqr) - handheld_Z * (HxNx + HyNy);
    double bearing_vec_mag = sqrt(pow(bearing_vec_X, 2) + pow(bearing_vec_Y, 2) + pow(bearing_vec_Z, 2));
    
    double dot_product = mag_north_vec_X * bearing_vec_X + mag_north_vec_Y * bearing_vec_Y + mag_north_vec_Z * bearing_vec_Z;
    bearing_to_node_deg = acos(dot_product / (mag_north_vec_mag * bearing_vec_mag)) * (180.0 / M_PI);
}

void vGPSRXUtility(void* pvParameters){
    GPSData_t gpsBench;
    // float lat2, lon2;

    while(1){
        if (xQueueReceive(guQueue, &gpsBench, portMAX_DELAY) == pdPASS) {
            if (gpsBench.systemSource == eHandheld){
                printf("\n===============GPS READING===============GPS Data Received from Handheld.\n%02u/%02u/%02u %02u:%02u:%02u UTC\nLatitude: %f, Longitude: %f\n=========================================\n",
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

