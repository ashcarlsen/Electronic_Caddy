#include "keypad.h"
#include "stm32l476xx.h" 
#include "Timer.h"
#include <stdlib.h>

static const unsigned int ODR_values[4] = {0x70,0xB0,0xD0,0xE0};

char keypadPoll(void)
{
    unsigned int i,compare, IDR_value;
    char key = 'z';
    for(i = 0; i < 4; i++){
        GPIOB->ODR |= 0xF0;
        GPIOB->ODR &= ODR_values[i];
				delay_ms(5);
        IDR_value = GPIOB->IDR;
        IDR_value = IDR_value>>8;
        IDR_value &= 0x0000000FU;
        if(IDR_value<15)
        {
            while(GPIOB->IDR == IDR_value);
            compare = (i*10)+IDR_value;
            switch(compare){
                case 7: key = '1'; break;//0b0111 char 1
                case 11:key = '2'; break;//0b1011 char 2
                case 13:key = '3'; break;//0xD char 3
                case 14:key = 'A'; break;//0XE char 'A'

                case 17:key = '4'; break;//char 4
                case 21:key = '5'; break;//char 5
                case 23:key = '6'; break;//char 6
                case 24:key = 'B'; break;//char 'B'

                case 27:key = '7'; break;//char 7
                case 31:key = '8'; break;//char 8
                case 33:key = '9'; break;//char 9
                case 34:key = 'C'; break;//char 'C'

                case 37:key = '*'; break;//char *
                case 41:key = '0'; break;//char 0
                case 43:key = '#'; break;//char #
                case 44:key = 'D'; break;//char 'D'
                default: break;
            }
        }
    }
    return key;
}

void SetupKeypad(void)
{
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	//enable clock on GPIOB
	GPIOB->MODER &= 0xFFFF00FF; // Setup B4-7 as outputs
	GPIOB->MODER |= 0x00005500;
	GPIOB->MODER &= 0xFF00FFFF; //GPIOB 8-11 inputs
	GPIOB->OTYPER |= 0x000000F0; // Set PB4-7 as output open-drain
}
	
int keypadInt(void)
{
    char buffer[10];
		for(int i = 0; i < 10; i++)
		{
			buffer[i] = '\0';
		}
    int index = 0;
    char data = keypadPoll();
		char* eptr = NULL;
    while(data != '#' && index < 10)
    {
        data = getChar();
        switch(data)
        {
            case 'A':
            case 'B':
            case 'C':
            case 'D':
            case '*':
            case '#': break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9': buffer[index] = data; index++; break;
            default: break;
        }
				delay_ms(310); // this is to avoid double inputs
    }
    int value = (int)strtol(buffer, &eptr, 10);
		return value;
}

char getChar(void)
{
	char key;
	do
	{
		key = keypadPoll();
	}while(key == 'z');
	return key;
}

char getAlpha(void)
{
	char key;
	uint8_t loop = 1;
	do
	{
		key = keypadPoll();
		if(key >= 'A' && key <= 'D')
		{
			loop = 0;
		}
	} while (loop);
	return key;
}
