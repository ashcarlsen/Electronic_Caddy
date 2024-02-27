#include "Distance.h"
#include <math.h>
#include <stdio.h>

#define DISTANCE_IN_YARDS 6974880
#define PI 3.141592

double distanceYds(double latDeg1, double lonDeg1, double latDeg2, double lonDeg2)
{
    double distance = 0.0;
    //Compute the Haversine distance equation with given lat/lon values in degrees
    // Convert to radians first
    double latRad1, lonRad1, latRad2, lonRad2 = 0.0;
    latRad1 = degToRad(latDeg1);
    lonRad1 = degToRad(lonDeg1);
    latRad2 = degToRad(latDeg2);
    lonRad2 = degToRad(lonDeg2);
    double temp, temp2 = 0.0;
    temp = sin(latRad1)*sin(latRad2);
    temp2 = cos(latRad1)*cos(latRad2)*cos(lonRad2-lonRad1);
    distance = acos(temp+temp2) * DISTANCE_IN_YARDS;
    // output the total distance in yards
    return distance;
}

//convert degrees to radians
double degToRad(double value)
{
    double rad = 0.0;
    rad = (value * PI)/180.0;
    return rad;
}
