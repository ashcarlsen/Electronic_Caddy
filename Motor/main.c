#include "stm32l476xx.h"
#include "Motor.h"

#define POS_COUNT 576
#define HALF_COUNT 280
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

void clubSpin(uint16_t club);
void clockwise(void);
void counter_clockwise(void);
void motorOff(void);

int main()
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                         {
	pos = 0;
	rotations = 0;
	club_pos = 1;
	EXTI_Init();
	prvMotorGPIO_Setup();
	// Set GPIOC pin 13 to input for Button
	GPIOC->MODER &= 0xF2FFFFFF;
	while(1)
	{
		clubSpin(7);
	}
	return 0;
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
				clockwise();
			}
			else
			{
				counter_clockwise();
			}
		}
		motorOff();
	}
}

void clockwise(void)
{
			GPIOC->ODR &= 0xFFFFFFFE; // C0 off
			GPIOC->ODR |= 0x00000008; // C3 on
}

void counter_clockwise(void)
{
			GPIOC->ODR &= 0xFFFFFFF7; //C3 off
			GPIOC->ODR |= 0x00000001; // C0 on
}

void motorOff(void)
{
	GPIOC->ODR &= 0xFFFFFFF7; //C3 off
	GPIOC->ODR &= 0xFFFFFFFE; // C0 off
}