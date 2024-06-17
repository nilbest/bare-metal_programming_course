/*
 * exti.c
 *
 *  Created on: Jun 17, 2024
 *      Author: nilsb
 */
#include "exti.h"

#define GPIOCEN			(1U<<2)
#define SYSCFGEN		(1U<<14)

void pc13_exti_init(void)
{
	//Disable global interrupt (Good practice)
	__disable_irq();

	//Enable clock access for GPIOC
	RCC->AHB1ENR |= GPIOCEN;

	//Set PC13 to input!!! (Explicit) to 00
	GPIOC->MODER &=~(1U<<26);
	GPIOC->MODER &=~(1U<<27);

	//Enable clock access to SYSCFG
	RCC->APB2ENR |= SYSCFGEN;

	//Select PORTC for EXTI13
	SYSCFG->EXTICR[3] = (1U<<5);

	//Unmask EXTI13
	EXTI->IMR |=(1U<<13);

	//Select falling edge trigger
	EXTI->FTSR |=(1U<<13);		//Changes Fallen Trigger Selection Register

	//Enable EXTI13 line in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn); //NVIC_EnableIRQ declared in core_cm4.h File
	//EXTI15_10_IRQn Symbolic name declared at stm32f411xe.h at 40

	//Enable global interrupts (Good practice)
	__enable_irq();
}

