#ifndef __STM32L476R_NUCLEO_KEYPAD_H
#define __STM32L476R_NUCLEO_KEYPAD_H

void SetupKeypad(void);
char keypadPoll(void); //Set ouput data register
int keypadInt(void);
char getChar(void);
char getAlpha(void);

#endif
