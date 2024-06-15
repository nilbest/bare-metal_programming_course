/*
 * uart.h
 *
 *  Created on: May 26, 2024
 *      Author: nilsb
 */

#ifndef UART_H_
#define UART_H_
#include<stdint.h>

#include "stm32f4xx.h"

void uart2_tx_init(void);

void uart2_rxtx_init(void);
char uart2_read(void);

#endif /* UART_H_ */
