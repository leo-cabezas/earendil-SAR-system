#ifndef TYPES_H
#define TYPES_H

#define GPSBUFFSIZE 70

struct moduleData{
  char gpsData[GPSBUFFSIZE];
  float bmpMetrics[3];
  float magMetrics[4];
  float gyroMetrics[6];
};

#endif
