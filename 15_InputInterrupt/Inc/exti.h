/*
 * exti.h
 *
 *  Created on: Jun 17, 2024
 *      Author: nilsb
 */

#ifndef EXTI_H_
#define EXTI_H_

#include "stm32f4xx.h"

void pc13_exti_init(void);

#define LINE13			(1U<<13)		//10.3.6 Pending register (EXTI_PR)

#endif /* EXTI_H_ */
