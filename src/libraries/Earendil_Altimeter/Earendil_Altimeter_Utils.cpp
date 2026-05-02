#include <Earendil_Altimeter.h>   // ATTENTION: Add all library dependencies to this header.

namespace Earendil_Altimeter {

    Adafruit_BMP3XX altimeter;

    void setup(){
        if (! altimeter.begin_I2C()) {   // hardware I2C mode, can pass in address & alt Wire
            //printf("Could not find a valid BMP3 sensor, check wiring!");
            return;
        }

          // Set up oversampling and filter initialization
        altimeter.setTemperatureOversampling(BMP3_OVERSAMPLING_2X);
        altimeter.setPressureOversampling(BMP3_OVERSAMPLING_16X);
        altimeter.setIIRFilterCoeff(BMP3_IIR_FILTER_COEFF_7);
        altimeter.setOutputDataRate(BMP3_ODR_1_5_HZ);
    }

    //Hypsometric equation function used to calculate the altitude based on temperature in Kelvin and pressure in hPa
    static inline float hypsometricAltitude(float temp, float pressure){
        float sea_level_rx = Earendil_Data->Radio_Data.rx_sea_level_hpa;
        if (sea_level_rx == -1){
            float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
            return altitude;
        }
        else{
            float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / sea_level_rx, HYPSOMETRIC_CONSTANT));
            return altitude;
        }
    }

    //Hypsometric equation function used to calculate the local sea level pressure based on temperature in Kelvin and pressure in hPa
    static inline float hypsometricPressure(float temp, float pressure){
      float sealevelpressure = (pressure) / (pow(1 - (LAPSE_RATE * LOCAL_ALTITUDE / temp), HYPSOMETRIC_CONSTANT));
      return sealevelpressure;
    }
    
    void altRead_hh(){
        if (! altimeter.performReading()) {
            //printf("Failed to perform reading :(");
            vTaskDelete(NULL);
        }

        float temp = altimeter.temperature;
        float press = altimeter.pressure / 100;
        float alt = hypsometricAltitude(temp + 273.15, press);

        
        Earendil_Data-> Altimeter_Data.temperature = temp;
        Earendil_Data-> Altimeter_Data.pressure = press;
        Earendil_Data-> Altimeter_Data.altitude = alt;

        //Print line for debugging if we need it
        //printf("\n============ALTIMETER READING============\nTemperature: %f *C\nPressure: %f hPa\nCalculated Altitude: %f m\n=========================================\n", Earendil_Data-> Altimeter_Data.temperature, Earendil_Data-> Altimeter_Data.pressure = press, Earendil_Data-> Altimeter_Data.altitude);

    }

    void altRead_node(){
        if (! altimeter.performReading()) {
            //printf("Failed to perform reading :(");
            vTaskDelete(NULL);
        }

        float temp = altimeter.temperature;
        float press = altimeter.pressure / 100;
        float sea_level = hypsometricPressure(temp + 273.15, press);

        Earendil_Data-> Altimeter_Data.temperature = temp;
        Earendil_Data-> Altimeter_Data.pressure = press;
        Earendil_Data-> Altimeter_Data.sea_level = sea_level;

        //Print line for debugging if we need it
        char buf[192];
        snprintf(buf, sizeof(buf), "\n============ALTIMETER READING============\nTemperature: %f *C\nPressure: %f hPa\nCalculated Sea-level Pressure: %f hPa\n=========================================\n",
            Earendil_Data-> Altimeter_Data.temperature, 
            Earendil_Data-> Altimeter_Data.pressure = press, 
            Earendil_Data-> Altimeter_Data.sea_level
        );
        puts(buf);        
    }

}