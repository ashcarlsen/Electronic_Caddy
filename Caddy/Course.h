#ifndef COURSE_HPP
#define COURSE_HPP
#include "stm32l476xx.h"

struct course
{
	char name[20];
	double latitudes[18];
	double longitudes[18];
	double altitudes[18];
};

#endif