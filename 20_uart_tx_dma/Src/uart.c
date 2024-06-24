/*
 * uart.c
 *
 *  Created on: May 26, 2024
 *      Author: nilsb
 */

#include "uart.h"

#define GPIOAEN		(1U<<0)
#define USART2EN	(1U<<17)

#define CR1_TE		(1U<<3)
#define CR1_RE		(1U<<2)

#define CR1_UE		(1U<<13)


#define DMA1EN		(1U<<21)
#define DMA_S_EN	(1U<<0)
#define CHSEL4		(1U<<27)
#define DMA_MINC	(1U<<10)
#define DMA_DIR_MEM_TO_PERIPH	(1U<<6)
#define DMA_CR_TCIE		(1U<<4)
#define UART_CR3_DMAT	(1U<<7)

#define SYS_FREQ	16000000
#define APB1_CLK	SYS_FREQ

#define CR1_RXNEIE		(1U<<5)
#define CR1_TXEIE		(1U<<7)

#define UART_BAUDRATE 115200

static void uart_set_baudrate(USART_TypeDef *UARTx, uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate);


void uart2_write(int ch);



int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}

void dma1_stream6_init(uint32_t src, uint32_t dst, uint32_t len)
{
	//Enable clock access to DMA
	RCC->AHB1ENR |=DMA1EN;
	//Disable DMA1 Stream 6
	DMA1_Stream6->CR &=~DMA_S_EN;

	//wait until DMA1 Stream 6 is disabled
	while(DMA1_Stream6->CR & DMA_S_EN){} //good to do so it has to be disabled...

	//Clear all interrupt falgs of Steam 6
	DMA1->HIFCR |= (1U<<16);
	DMA1->HIFCR |= (1U<<18);
	DMA1->HIFCR |= (1U<<19);
	DMA1->HIFCR |= (1U<<20);
	DMA1->HIFCR |= (1U<<21);

	//Set destination buffer DMA_SxPAR
	DMA1_Stream6->PAR = dst;
	//Set source buffer DMA_SxM0AR
	DMA1_Stream6->M0AR = src;

	//Set lenght
	DMA1_Stream6->NDTR = len;

	//Select Stream6 CH4
	DMA1_Stream6->CR = CHSEL4; //Clears register and passes CHSEL4 into it
	//Enable memory increment
	DMA1_Stream6->CR |= DMA_MINC; //Or to adding only this bit
	//Configure transfer direction
	DMA1_Stream6->CR |= DMA_DIR_MEM_TO_PERIPH;
	//Enable DMA Transfer complete interrrupt
	DMA1_Stream6->CR |= DMA_CR_TCIE;

	//Enbale direct mode and disable FIFO
	DMA1_Stream6->FCR = 0;

	//Enable DMA1 Steam6
	DMA1_Stream6->CR |= DMA_S_EN;
	//Enable UART2 transmitter DMA
	USART2->CR3 |= UART_CR3_DMAT;
	//DMA Interrupt enable in NVIC NVIC_EnableIRQ(DMA)
	NVIC_EnableIRQ(DMA1_Stream6_IRQn);
}

void uart2_rx_interrupt_init(void)
{
	//***********************Configure uart gpio pin***********************
	//Enable clock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;
	//Set PA2 mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	//Set PA2 alternate function type to UART_TX (AF07) => In AFRL=AFR[0] in .h file
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	//Set PA3 mode to alternate function mode
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	//Set PA3 alternate function type to UART_RX (AF07) => In AFRL=AFR[0] in .h file
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);
	//***********************Configure uart module***********************
	//Enable clock access to uart2
	RCC->APB1ENR |= USART2EN;
	//Configure baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	//Configure the transfer direction
	USART2->CR1 = (CR1_TE | CR1_RE); //Everything else is cleaned

	//Enables RXNE Interrupt
	USART2->CR1 |= CR1_RXNEIE;

	//Enable TXNE Interrupt
	//USART2->CR1 |= CR1_TXEIE;

	//Enable UART2 interrupt in NVIC
	NVIC_EnableIRQ(USART2_IRQn);

	//Enable uart module
	USART2->CR1 |= CR1_UE;
}

void uart2_rxtx_init(void)
{
	//***********************Configure uart gpio pin***********************
	//Enable clock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;
	//Set PA2 mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	//Set PA2 alternate function type to UART_TX (AF07) => In AFRL=AFR[0] in .h file
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);

	//Set PA3 mode to alternate function mode
	GPIOA->MODER &=~(1U<<6);
	GPIOA->MODER |= (1U<<7);
	//Set PA3 alternate function type to UART_RX (AF07) => In AFRL=AFR[0] in .h file
	GPIOA->AFR[0] |= (1U<<12);
	GPIOA->AFR[0] |= (1U<<13);
	GPIOA->AFR[0] |= (1U<<14);
	GPIOA->AFR[0] &=~(1U<<15);
	//***********************Configure uart module***********************
	//Enable clock access to uart2
	RCC->APB1ENR |= USART2EN;
	//Configure baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	//Configure the transfer direction
	USART2->CR1 = (CR1_TE | CR1_RE); //Everything else is cleaned
	//Enable uart module
	USART2->CR1 |= CR1_UE;
}

void uart2_tx_init(void)
{
	//***********************Configure uart gpio pin***********************
	//Enable clock access to gpioa
	RCC->AHB1ENR |= GPIOAEN;
	//Set PA mode to alternate function mode
	GPIOA->MODER &=~(1U<<4);
	GPIOA->MODER |= (1U<<5);
	//Set PA2 alternate function type to UART_TX (AF07) => In AFRL=AFR[0] in .h file
	GPIOA->AFR[0] |= (1U<<8);
	GPIOA->AFR[0] |= (1U<<9);
	GPIOA->AFR[0] |= (1U<<10);
	GPIOA->AFR[0] &=~(1U<<11);


	//***********************Configure uart module***********************
	//Enable clock access to uart2
	RCC->APB1ENR |= USART2EN;
	//Configure baudrate
	uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	//Configure the transfer direction
	USART2->CR1 = CR1_TE; //Everything else is cleaned
	//Enable uart module
	USART2->CR1 |= CR1_UE;
}

char uart2_read(void)
{
	//Make sure the receive data register is  not empty
	while(!(USART2->SR & SR_RXNE));

	//Read Data
	return USART2->DR;
}

void uart2_write(int ch)
{
	//Make sure the transmit data register is empty
	while(!(USART2->SR & SR_TXE)); //SR => Status Register
	//write to transmit data register
	USART2->DR = (ch & 0xFF); //DR => Data Register
}

static void uart_set_baudrate(USART_TypeDef *UARTx, uint32_t PeriphClk, uint32_t BaudRate)
{
	UARTx->BRR = compute_uart_bd(PeriphClk, BaudRate); //BRR => Baud Rate Register
}

static uint16_t compute_uart_bd(uint32_t PeriphClk, uint32_t BaudRate)
{
	return ((PeriphClk + (BaudRate/2U))/BaudRate); //2U => 2 as Unsigned integer
}
