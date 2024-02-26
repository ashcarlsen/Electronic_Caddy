#ifndef GPS_H
#define GPS_H
#include "stm32l476xx.h"
#define BUFFER_SIZE 350
#define GPGGA_SIZE 96

struct GPPGA
{
	float m_time;
	float m_lat;
	float m_lon;
	float m_alt;
};


void GPS_Init(void);
void GPS_Read_NMEA(char *buffer, uint32_t size);
void GPS_GPIO_Init(void);

#endif