#ifndef MAG_H
#define MAG_H

#include "types.h"

struct Magmeter{
  public:
    float hard_min[3] = {9999, 9999, 9999}; //ensures its crazy out of scope from any actual values so it can be filled
    float hard_max[3] = {-9999, -9999, -9999};
    float hard_iron[3] = {0.0, 0.0, 0.0};

    float soft_min[3] = {999, 999, 999};
    float soft_max[3] = {-999, -999, -999};
    float soft_iron[3][3];

};

void magnetoSetup();

void magnetoReading(float* magMetrics);

void magnetoShow(float* magMetrics);

#endif