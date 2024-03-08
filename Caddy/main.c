#include "stm32l476xx.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "GPS.h"
#include "LCD.h"
#include "Parse.h"
#include "Timer.h"
#include "keypad.h"
#include "flash.h"
#include "Course.h"
#include "UTC.h"

static char rxBuffer[BUFFER_SIZE] ={0};
static uint16_t clubs[12] = {0};
static uint8_t courseNumber = 0;
static uint8_t holeNumber = 0;
static const struct course courseList[4] = {
{
	.name = "Engineering Test", 
	.latitudes = {41.748608, 41.748473, 41.748500}, 
	.longitudes = {-111.819925, -111.819952, -111.819737}, 
	.altitudes = {0, 0, 0}
},
{.name = "Logan River Golf", .latitudes = {0}, .longitudes = {0}, .altitudes = {0}},
{.name = "Preston Golf", .latitudes = {0}, .longitudes = {0}, .altitudes = {0}},
{.name = "Montpelier Golf", .latitudes = {0}, .longitudes = {0}, .altitudes = {0}},
};

void mainMenu(void);
void editClubs(void);
void selectCourse(void);
void play(void);
void playManual(void);
uint8_t chooseClub(uint16_t distance);

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
	SetupKeypad();
	readClubs(clubs);

  while(1)
	{
		mainMenu();
		playManual();
		LCD_Clear();
  }
}

void mainMenu(void)
{
	char selection = '\0';
	LCD_DisplayString(0, "     Main Menu:", 15);
	LCD_DisplayString(2, "1. Select Course\0", 17);
	LCD_DisplayString(3, "2. Edit Yardages\0", 17); 
	selection = keypadPoll();
	while(selection == 'z')
	{ 
		selection = keypadPoll();
		if(!(selection == '1' || selection == '2'))
		{
			selection = 'z';
		}
	}
	LCD_Clear();
	if(selection == '1')
	{
		LCD_DisplayString(0, "Select Course", 13);
		selectCourse();
	}
	else
	{
		LCD_DisplayString(0, "Edit Yardages", 13);
		editClubs();
	}
}

void editClubs(void)
{
	char key;
	for(int i = 1; i <= 12; i++)
	{
		char buffer[20] = {0};
		char buffer2[20] = {0};
		sprintf(buffer, "Edit Club #%d:", i);
		LCD_Clear();
		do
		{
			LCD_DisplayString(0, buffer, 20);
			uint16_t val = keypadInt();
			sprintf(buffer2, "%d?", val);
			LCD_DisplayString(1, buffer2, 20);
			LCD_DisplayString(2, "Yes: #", 6);
			LCD_DisplayString(3, "No: *", 5);
			key = getChar();
			if(key == '#')
			{
				clubs[i-1] = val;
			}
			else
			{
				LCD_Clear();
			}
		}while(key != '#');
		delay_ms(20);
	}
	writeClubs(clubs);
}

void selectCourse(void)
{
	char key;
	char buffer[20] = {0};
	char buffer2[20] = {0};
	char buffer3[20] = {0};
	char buffer4[20] = {0};
	sprintf(buffer, "A. %s", courseList[0].name);
	sprintf(buffer2, "B. %s", courseList[1].name);
	sprintf(buffer3, "C. %s", courseList[2].name);
	sprintf(buffer4, "D. %s", courseList[3].name);
	LCD_DisplayString(0, buffer, 20);
	LCD_DisplayString(1, buffer2, 20);
	LCD_DisplayString(2, buffer3, 20);
	LCD_DisplayString(3, buffer4, 20);
	key = getAlpha();
	switch(key)
	{
		case 'A': courseNumber = 0; break;
		case 'B': courseNumber = 1; break;
		case 'C': courseNumber = 2; break;
		case 'D': courseNumber = 3; break;
		default: break;
	}
}

void play(void)
{
	LCD_Clear();
	holeNumber = 1;
	char data[GPGGA_SIZE];
	char *fields[15];
	double latitude = 0.0f;
	double longitude = 0.0f;
	double courseLat = 0.0f;
	double courseLon = 0.0f;
	char key = 0;
	do
	{
		key = 0;
		GPS_Read_NMEA(rxBuffer, BUFFER_SIZE);
		getGPGGA(rxBuffer, data, GPGGA_SIZE);
		parseGPGGA(data, fields);
		latitude = nmeaToDeg(fields[2]);
		longitude = nmeaToDeg(fields[4]);
		if(strcmp(fields[5], "W") == 0)
		{
			longitude = longitude * -1;
		}
		courseLat = courseList[courseNumber].latitudes[holeNumber-1];
		courseLon = courseList[courseNumber].longitudes[holeNumber-1];
		int fix = atoi(fields[6]);
		if(fix != 0)
		{
			uint16_t distance = distanceYds(latitude, longitude, courseLat, courseLon);
			if(distance > 999)
			{
				distance = 0;
			}
			uint8_t index = chooseClub(distance);
			char holeInfo[20] = {0};
			char distInfo[20] = {0};
			char timeInfo[20] = {0};
			sprintf(holeInfo, "Hole: %2d", holeNumber);
			sprintf(distInfo, "%3d yards  Club: %2d", distance, index);
			utcToMST(fields[1], timeInfo);
			LCD_DisplayString(0, courseList[courseNumber].name, 20);
			LCD_DisplayString(1, holeInfo, 20);
			LCD_DisplayString(2, distInfo, 20);
			LCD_DisplayString(3, timeInfo, 20);
		}
		else
		{
			LCD_Clear();
			LCD_DisplayString(0, "No GPS Fix", 10);
			delay_ms(1000);
			LCD_Clear();
		}
		key = keypadPoll();
		switch(key)
		{
			case 'A': if(holeNumber < 18){holeNumber--;} break;
			case 'B': if(holeNumber > 1){holeNumber++;} break;
			default: break;
		}
	} while(key != '*');
}

void playManual(void)
{
	LCD_Clear();
	holeNumber = 1;
	char data[GPGGA_SIZE];
	char *fields[15];
	double latitude = 0.0f;
	double longitude = 0.0f;
	double courseLat = 0.0f;
	double courseLon = 0.0f;
	char key = 0;
	do
	{
		key = 'z';
		GPS_Read_NMEA(rxBuffer, BUFFER_SIZE);
		getGPGGA(rxBuffer, data, GPGGA_SIZE);
		parseGPGGA(data, fields);
		latitude = nmeaToDeg(fields[2]);
		longitude = nmeaToDeg(fields[4]);
		if(strcmp(fields[5], "W") == 0)
		{
			longitude = longitude * -1;
		}
		courseLat = courseList[courseNumber].latitudes[holeNumber-1];
		courseLon = courseList[courseNumber].longitudes[holeNumber-1];
		int fix = atoi(fields[6]);
		if(fix != 0)
		{
			uint16_t distance = distanceYds(latitude, longitude, courseLat, courseLon);
			if(distance > 999)
			{
				distance = 0;
			}
			uint8_t index = chooseClub(distance);
			char holeInfo[20] = {0};
			char distInfo[20] = {0};
			sprintf(holeInfo, "Hole: %2d", holeNumber);
			sprintf(distInfo, "%3d yards  Club: %2d", distance, index);
			LCD_DisplayString(0, courseList[courseNumber].name, 20);
			LCD_DisplayString(1, holeInfo, 20);
			LCD_DisplayString(2, distInfo, 20);
			key = getChar();
			switch(key)
			{
				case 'A' : if(holeNumber < 18){holeNumber++;} break;
				case 'B' : if(holeNumber > 1){holeNumber--;} break;
				default: break;
			}
		}
		else
		{
			LCD_Clear();
			LCD_DisplayString(0, "No GPS Fix", 10);
			delay_ms(1000);
			LCD_Clear();
			key = keypadPoll();
		}
	} while(key != '*');
}

uint8_t chooseClub(uint16_t distance)
{
	uint16_t diff = 65535;
	uint16_t newDiff;
	uint8_t index = 0;
	for(int i = 0; i < 12; i++)
	{
		newDiff = abs(clubs[i]-distance);
		if(newDiff <= diff)
		{
			diff = newDiff;
			index = i;
		}
	}
	// Add one because Club numbers start at 1
	return index + 1;
}
