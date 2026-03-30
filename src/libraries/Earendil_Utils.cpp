#include <Earendil_Utils.h>

void vGetAltitude(void* pvParameters){
  float altitude = (temp / LAPSE_RATE) * (1 - pow(pressure / SEALEVELPRESSURE_HPA, HYPSOMETRIC_CONSTANT));
  return altitude;
}

void vGetPressure(void* pvParameters);