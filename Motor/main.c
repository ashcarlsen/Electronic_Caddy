#include "stm32l476xx.h"
#include "Motor.h"

static int pos;
static int rotations;

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
		if(pos >= 576)
		{
			rotations += 1;
			pos = 0;
		}
		else if(pos <= -576)
		{
			rotations -= 1;
			pos = 0;
		}
		EXTI->PR1 |= EXTI_PR1_PIF1;
	}
}

void clockwise(void);
void counter_clockwise(void);

int main()
{
	pos = 0;
	rotations = 0;
	EXTI_Init();
	prvMotorGPIO_Setup();
	// Set GPIOC pin 13 to input for Button
	GPIOC->MODER &= 0xF2FFFFFF;
	while(1)
	{
		if((GPIOC->IDR & 0x00002000) == 0)
		{
			clockwise();
		}
		else
		{
			GPIOC->ODR &= 0xFFFFFFF7; //C3 off
			//counter_clockwise();
		}
	}
	return 0;
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