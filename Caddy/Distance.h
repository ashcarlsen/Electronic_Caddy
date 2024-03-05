#ifndef DISTANCE_H
#define DISTANCE_H
#include "stm32l476xx.h"

uint16_t distanceYds(double latDeg1, double lonDeg1, double latDeg2, double lonDeg2);
double degToRad(double value);

#endif