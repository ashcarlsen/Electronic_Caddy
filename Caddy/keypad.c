#include "keypad.h"
#include "stm32l476xx.h" 

static const unsigned int ODR_values[4] = {0x70,0xB0,0xD0,0xE0};

char keypadPoll(void)
{
    unsigned int i,compare, IDR_value;
    char key = 'z';
    for(i = 0; i < 4; i++){
        GPIOB->ODR |=0xF0;
        GPIOB->ODR &= ODR_values[i];
		keydelay_ms(5);
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

                case 37:key = '\0'; break;//char *
                case 41:key = '0'; break;//char 0
                case 43:key = '#'; break;//char #
                case 44:key = 'D'; break;//char 'D'
                default: break;
            }
        }
    }
    return key;
}
