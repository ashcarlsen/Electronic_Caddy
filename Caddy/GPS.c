#include "GPS.h"
#include "stm32l476xx.h"
#include <string.h>
#include <stdio.h>

/*============================
*	Pins:
*	B7: USART1 Rx
*============================*/

void GPS_Read_NMEA(char *buffer, uint32_t size)
	{
	USART1->CR1 |= USART_CR1_UE; //enable USART
	while((USART1->ISR & USART_ISR_REACK) == 0); //recieve enable flag
	unsigned int i = 0;
	char done = 0;
	char valid = 0;
	for(unsigned int k = 0; k < size; k++)	// Initialize buffer to nulls
	{
		buffer[k] = '\0';
	}
	for(int i = 0; i < size; i++)	//Read into the entire buffer
	{
		while(!(USART1->ISR & USART_ISR_RXNE));
		buffer[i] = USART1->RDR;
		if(buffer[i] == '$')
		{
			valid = 1;
		}
		else if(valid == 0)
		{
			i = 0;
			valid = 0;
		}
		else
		{
			if (strstr(buffer, "GPGGA") != NULL && buffer[i] == '\n')	// If we get enough data just stop reading
			{
				for(int j = i+1; j < size; j++)
				{
					buffer[j] = '\0';
				}			
				i = size;
			}
		}
	}
	USART1->CR1 &= ~USART_CR1_UE; //disable USART
}

void GPS_Init(void){
	GPS_GPIO_Init();
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //Enable UART Clock
	USART1->CR1 &= ~USART_CR1_UE; //disable USART
	USART1->CR1 &= ~(USART_CR1_M); //set to 8-bit word, 1 start, n stop bits
	USART1->CR2 &= ~(USART_CR2_STOP); //set to 1 stop bit
	USART1->CR1 &= ~(USART_CR1_PCE); //disable parity control bits
	USART1->CR1 &= ~(USART_CR1_OVER8); //clear over8 bit to 16 oversampling
	USART1->BRR = 0x682; //9600 baud
	USART1->CR1 |= (USART_CR1_RE); //enable transmit and receive
	USART1->CR1 |= USART_CR1_UE; //enable USART
	while((USART1->ISR & USART_ISR_REACK) == 0); //recieve enable flag
}

void GPS_GPIO_Init(void){
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;	//enable clock on GPIOA
	GPIOB->MODER &= ~GPIO_MODER_MODE7;
	GPIOB->MODER |= GPIO_MODER_MODE7_1; //sets pin B7 to analog function
	GPIOB->AFR[0] |= 0x77 << (4*6);		// Sets pinB6-7 as analog if we want TX too
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD7);
	GPIOB->PUPDR |= (GPIO_PUPDR_PUPD7_0); //Set as Pull-up
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED7); //Set as high speed
	GPIOB->OTYPER &= ~(GPIO_OTYPER_OT7);	// Push-pull output type
}