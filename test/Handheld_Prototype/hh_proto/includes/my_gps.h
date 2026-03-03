#ifndef MY_GPS_H
#define MY_GPS_H

#include "types.h"

void my_gpsSetup();

void gpsReading(char* gpsData, int buffSize);

void gpsShow(char* gpsData, int buffSize);

#endif