#ifndef BMP_H
#define BMP_H

#include "types.h"

void bmpSetup();

void bmpReading(float* bmpMetrics);

void bmpShow(float* bmpMetrics);

float hypsometric_altitude(float temp, float pressure);

#endif