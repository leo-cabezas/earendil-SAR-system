#pragma once

#define GPSBUFFSIZE 64

typedef struct ModuleData {
  float bmpMetrics[4];
  float magMetrics[4];
  float gyroMetrics[6];
  char gpsData[GPSBUFFSIZE];
} ModuleData_t;

typedef struct GPSData {
  double latitude_handheld;
  double longitude_handheld;
  double altitude_handheld;
  double latitude_node;
  double longitude_node;
  double altitude_node;
} GPSData_t;

// typedef struct RadioData {
//   uint8_t RECV[RH_RF95_MAX_MESSAGE_LEN];
//   int16_t lastRSSI;
// } RadioData_t;

typedef struct Handheld_Shared {
  ModuleData_t* sensorData;
  GPSData_t* gpsData;
  //RadioData_t* radioData;
  SemaphoreHandle_t* g_printMutex;  
} Handheld_Shared_t;