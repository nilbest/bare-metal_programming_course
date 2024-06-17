#include <stdio.h>
#include <stdint.h>
#include "stm32f4xx.h"
#include "uart.h"
#include "adc.h"
#include "systick.h"
#include "tim.h"
#include "exti.h"


#define GPIOAEN					(1U<<0)
#define PIN5					(1U<<5)
#define LED						(PIN5)


static void exti_callback(void);

int main(void)
{
	//Enable clock access to GPIOA
	RCC->AHB1ENR |= GPIOAEN;

	//Set PA5 as output pin
	GPIOA->MODER |= (1U<<10);
	GPIOA->MODER &=~(1U<<11);

	uart2_tx_init();
	pc13_exti_init();

	while(1)
	{

	}
}



static void exti_callback(void)
{
	printf("BTN Pressed...\n\r");
	GPIOA->ODR ^= LED;				//Toggle
}



void EXTI15_10_IRQHandler(void)				//Name of the Vector in the Startup/startup_stm32f411retx.s from the g_pfnVectors => Handles all Interrupts
{
	if((EXTI->PR & LINE13)!=0)			//Checks if EXTI PR Register at Place LINE13 is not 0
	{
		//Clear PR flag
		EXTI->PR |= LINE13;
		//Do something
		exti_callback();
	}
}


