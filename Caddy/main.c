#include "stm32l476xx.h"
#include <string.h>
#include "GPS.h"
#include "LCD.h"
#include "Parse.h"
#include "Timer.h"

static char rxBuffer[BUFFER_SIZE] ={0};

int main(void)
{
	RCC->CR |= RCC_CR_HSION;            // enable HSI (internal 16 MHz clock)
  while ((RCC->CR & RCC_CR_HSIRDY) == 0);
  RCC->CFGR |= RCC_CFGR_SW_HSI;    // make HSI the system clock
	SystemCoreClockUpdate();
	SetupTIM4();	// Setup TIM4 to be able to delay. MUST be done before LCD
	LCD_Init();
	LCD_Clear();
	GPS_Init();
	char data[GPGGA_SIZE];
	char *fields[15];
	char lat[20] = {0};
	char lon[20] = {0};
	char alt[20] = {0};
	char time[20] = {0};
	for(int i = 0; i < 20; i++)
	{
		lat[i] = '\0';
		lon[i] = '\0';
		alt[i] = '\0';
		time[i] = '\0';
	}
  while(1)
	{
		GPS_Read_NMEA(rxBuffer, BUFFER_SIZE);
		getGPGGA(rxBuffer, data, GPGGA_SIZE);
		parseGPGGA(data, fields);
		getLatString(fields[2], strlen(fields[2]), lat);
		getLonString(fields[4], strlen(fields[4]), lon);
		getAltString(fields[9], strlen(fields[9]), alt);
		getTimeString(fields[1], strlen(fields[1]), time);
		LCD_Clear();
		LCD_DisplayString(0, lat, 20);
		LCD_DisplayString(1, lon, 20);
		LCD_DisplayString(2, alt, 20);
		LCD_DisplayString(3, time, 20);
  }
}

