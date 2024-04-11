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
#include "Motor.h"
#include "UTC.h"

#define MODE_INDEX 0
#define PREF_INDEX 1

static uint16_t test = 1;

static char rxBuffer[BUFFER_SIZE] ={0};
static uint16_t clubs[12] = {0};
static uint8_t courseNumber = 0;
static uint8_t holeNumber = 0;
static uint16_t settings[2] = {0};

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

#define TEN_COUNT 56

static int pos;
static int rotations;
static uint16_t club_pos;

void EXTI_Init(void) {
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI1; 
	SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI1_PC;

	EXTI->RTSR1 |= EXTI_RTSR1_RT1;

	EXTI->IMR1 |= EXTI_IMR1_IM1;
	NVIC_EnableIRQ(EXTI1_IRQn);
}

void EXTI1_IRQHandler(void){
	if((EXTI->PR1 & EXTI_PR1_PIF1) == EXTI_PR1_PIF1){
		if(GPIOC->IDR & 0x00000004)
		{
			pos = pos + 1;
		}
		else
		{
			pos = pos - 1;
		}
		if(pos >= TEN_COUNT)
		{
			rotations += 1;
			if(rotations >= 4)
			{
				if(club_pos == 12)
				{
					club_pos = 1;
				}
				else
				{
					club_pos += 1;
				}
				rotations = 0;
			}
			pos = 0;
		}
		else if(pos <= -TEN_COUNT)
		{
			rotations -= 1;
			if(rotations <= -4)
			{
				if(club_pos == 1)
				{
					club_pos = 12;
				}
				else
				{
					club_pos -= 1;
				}
				rotations = 0;
			}
			pos = 0;
		}
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
}

void mainMenu(void);
void editClubs(void);
void editSettings(void);
void selectCourse(void);
void playManual(void);
uint8_t chooseClub(uint16_t distance);
void clubSpin(uint16_t club);

int main(void)
{
	pos = 0;
	rotations = 0;
	RCC->CR |= RCC_CR_HSION;            // enable HSI (internal 16 MHz clock)
 	while ((RCC->CR & RCC_CR_HSIRDY) == 0);
 	RCC->CFGR |= RCC_CFGR_SW_HSI;    // make HSI the system clock
	SystemCoreClockUpdate();
	SetupTIM4();	// Setup TIM4 to be able to delay. MUST be done before LCD
	EXTI_Init();
	prvMotorGPIO_Setup();
	LCD_Init();
	LCD_Clear();
	GPS_Init();
	SetupKeypad();
	readClubs(clubs);
	readSettings(settings);
	club_pos = readPosition();

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
	LCD_DisplayString(1, "1. Select Course\0", 17);
	LCD_DisplayString(2, "2. Edit Yardages\0", 17); 
	LCD_DisplayString(3, "3. Edit Settings\0", 17);
	selection = keypadPoll();
	// Change this to getChar()
	while(selection == 'z')
	{ 
		selection = getChar();
		if(!(selection == '1' || selection == '2' || selection == '3'))
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
	else if(selection == '2')
	{
		LCD_DisplayString(0, "Edit Yardages", 13);
		editClubs();
	}
	else
	{
		LCD_DisplayString(0, "Edit Settings", 13);
		editSettings();
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
			clubSpin(i);
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

void editSettings(void)
{
	char key = 0;
	char buffer[20] = {0};
	do
	{
		LCD_Clear();
		LCD_DisplayString(0, "Edit Mode", 9);
		LCD_DisplayString(1, "Manual Mode: A", 14);
		LCD_DisplayString(2, "Auto Mode: B", 12);
		key = getChar();
		if(key == 'A')
		{
			settings[MODE_INDEX] = 0;
		}
		else if(key == 'B')
		{
			settings[MODE_INDEX] = 1;
		}
		else if(key == '*')
		{
			key = 1;
		}
		else
		{
			key = 0;
		}
	} while(key == 0);
	do
	{
		LCD_Clear();
		LCD_DisplayString(0, "Edit Preference", 15);
		LCD_DisplayString(1, "Club Up: A", 10);
		LCD_DisplayString(2, "Club Down: B", 12);
		key = getChar();
		if(key == 'A')
		{
			settings[PREF_INDEX] = 0;
		}
		else if(key == 'B')
		{
			settings[PREF_INDEX] = 1;
		}
		else if(key == '*')
		{
			key = 1;
		}
		else
		{
			key = 0;
		}
	} while(key == 0);
	writeSettings(settings);
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
			clubSpin(index);
			char holeInfo[20] = {0};
			char distInfo[20] = {0};
			char timeInfo[20] = {0};
			sprintf(holeInfo, "Hole: %2d", holeNumber);
			sprintf(distInfo, "%3d yards  Club: %2d", distance, index);
			if(settings[MODE_INDEX] == 1)
			{
				utcToMST(fields[1], timeInfo);
				LCD_DisplayString(3, timeInfo, 20);
			}
			LCD_DisplayString(0, courseList[courseNumber].name, 20);
			LCD_DisplayString(1, holeInfo, 20);
			LCD_DisplayString(2, distInfo, 20);
			if(settings[MODE_INDEX] == 0)
			{
				key = getChar();
			}
			else
			{
				key = keypadPoll();
			}
			switch(key)
			{
				case 'A' : if(holeNumber < 18){holeNumber++;} break;
				case 'B' : if(holeNumber > 1){holeNumber--;} break;
				case 'C' : break;
				case 'D' : break;
				case '#' : break;
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
		if(settings[PREF_INDEX] == 0)
		{
			if(newDiff <= diff)
			{
				diff = newDiff;
				index = i;
			}
		}
		else
		{
			if(newDiff < diff)
			{
				diff = newDiff;
				index = i;
			}
		}
	}
	// Add one because Club numbers start at 1
	return index + 1;
}

void clubSpin(uint16_t club)
{
	int cur_pos = club_pos;
	uint8_t direction = 0;
	if(cur_pos > club)
	{
		if((cur_pos - club) < 6)
		{
			direction = 0;
		}
		else
		{
			direction = 1;
		}
	}
	else
	{
		if((club - cur_pos) < 6)
		{
			direction = 1;
		}
		else
		{
			direction = 0;
		}
	}
	if(club <= 12 && club >= 1)
	{
		while(club_pos != club)
		{
			if(direction)
			{
				counter_clockwise();
			}
			else
			{
				clockwise();
			}
		}
		motorOff();
	}
	writePosition(club_pos);
}