#include<stdio.h>
#include<stdint.h>
#include "stm32f4xx.h"
#include "uart.h"


#define GPIOAEN					(1U<<0) // Shifts 1 at position 0 => 0b 0000 0000 0000 0000 0000 0000 0000 0001
#define GPIOA_5					(1U<<5)
#define LED_PIN					GPIOA_5


char key;

static void uart_callback(void);

int main(void)
{
	//1. Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN; // |= Or operator changes only bit set at GPIOAEN to 1 =>friendly programming
	//2. Set PA5 as output pin
	GPIOA->MODER |= (1U<<10);	// Set bit 10 to 1
	GPIOA->MODER &=~(1U<<11);	// Set bit 11 to 0  &and operation with current register ~negate


	uart2_rx_interrupt_init();

	while(1)
	{

	}

}

static void uart_callback(void)
{
	key = USART2->DR;
	if (key == '1')
	{
		GPIOA->ODR |= LED_PIN;
	}
	else
	{
		GPIOA->ODR &= ~LED_PIN;
	}
}


void USART2_IRQHandler(void)
{
	//Check if RXNE is set
	if(USART2->SR & SR_RXNE)
	{
		//Do something
		uart_callback();
	}
	//if(USART2->SR & SR_TXE)
	//{
		//Do something
		//printf("Test!\n\r");
	//}

}



