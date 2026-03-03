#include "../includes/my_gps.h"
#include <SoftwareSerial.h>
#include <Adafruit_GPS.h>

#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F"
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C"
#define PMTK_SET_NMEA_UPDATE_10HZ "$PMTK220,100*2F"
// turn on only the second sentence (GPRMC)
#define PMTK_SET_NMEA_OUTPUT_RMCONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
// turn on GPRMC and GGA
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn on ALL THE DATA
#define PMTK_SET_NMEA_OUTPUT_ALLDATA "$PMTK314,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
// turn off output
#define PMTK_SET_NMEA_OUTPUT_OFF "$PMTK314,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"
#define PMTK_Q_RELEASE "$PMTK605*31"

SoftwareSerial GPS_UART(1, 0); //GPS_UART(GPS TX Connects to this pin on the uC, GPS RX connects to this pin on the uC)

void my_gpsSetup(){ //GPS Setup Function
  GPS_UART.begin(9600);
  delay(2000);
  GPS_UART.println(PMTK_SET_NMEA_OUTPUT_RMCONLY);
  GPS_UART.println(PMTK_SET_NMEA_UPDATE_1HZ);
  Serial.println("Get version!");
  GPS_UART.println(PMTK_Q_RELEASE);
}

void gpsReading(char* gpsData, int buffSize){
  int index = 0;

  while (1){
    if (GPS_UART.available() && (GPS_UART.read() == '$')){
      gpsData[0] = '$';
      index++;
      break;
    }
  }

  while (index < buffSize - 1) {  // Leave room for null terminator
      if (GPS_UART.available()) {
          char c = GPS_UART.read();
          gpsData[index++] = c;
          if (c == '*') {  //Stop reading at the parity bits
              break;
          }
      }
  }
  gpsData[index] = '\0';  // Null terminate
}

void gpsShow(char* gpsData, int buffSize){
  for (int i = 0; i < buffSize; i++){
    Serial.print(gpsData[i]);
  }
  Serial.println();
}