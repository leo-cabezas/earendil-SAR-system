#ifndef GYRO_H
#define GYRO_H

#include "types.h"

void gyroSetup();

void gyroReading(float* gyroMetrics);

void gyroShow(float* gyroMetrics);

#endif