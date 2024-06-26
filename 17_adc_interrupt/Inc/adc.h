/*
 * uart.h
 *
 *  Created on: May 26, 2024
 *      Author: nilsb
 */

#ifndef ADC_H_
#define ADC_H_

#include<stdint.h>
#include "stm32f4xx.h"

#define SR_EOC			(1U<<1)

void pa1_adc_interrupt_init(void);
void pa1_adc_init(void);
void start_conversion(void);
uint32_t adc_read(void);

#endif /* ADC_H_ */
