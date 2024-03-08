#include "stm32l476xx.h"
#include <stdlib.h>
#include <stdio.h>
#include "LCD.h"
#include "timer.h"
#include "GPS.h"
#include "Parse.h"
#include "keypad.h"
#include "UTC.h"

static char rxBuffer[BUFFER_SIZE] ={0};

int main(void){
	RCC->CR |= RCC_CR_HSION;	// turn on HSI
	while((RCC->CR & RCC_CR_HSIRDY) == 0);	//wait till HSI is ready
	RCC->CFGR |= RCC_CFGR_SW_HSI;    // make HSI the system clock
	SystemCoreClockUpdate();
	SetupTIM4();
	LCD_Init();
	LCD_Clear();
	GPS_Init();
	SetupKeypad();
	char data[GPGGA_SIZE];
	char *fields[15];
	char buffer[20] = {0};
	char buffer2[20] = {0};
	char buffer3[20] = {0};
	char buffer4[20] = {0};
	char key = 0;
	while(1)
	{
		GPS_Read_NMEA(rxBuffer, BUFFER_SIZE);
		getGPGGA(rxBuffer, data, GPGGA_SIZE);
		parseGPGGA(data, fields);
		utcToMST(fields[1], buffer4);
		sprintf(buffer, "Lat: %s", fields[2]);
		sprintf(buffer2, "Lon: %s", fields[4]);
		sprintf(buffer3, "Fix: %s %s", fields[6], fields[7]);
		LCD_DisplayString(0, buffer, 20);
		LCD_DisplayString(1, buffer2, 20);
		LCD_DisplayString(2, buffer3, 20);
		LCD_DisplayString(3, buffer4, 20);
		key = getChar();
	}
}