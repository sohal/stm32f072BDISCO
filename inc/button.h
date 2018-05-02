/*
 * button.h
 *
 *  Created on: Jan 19, 2018
 *      Author: p
 */

#ifndef BUTTON_H_
#define BUTTON_H_

#include "stm32f0xx_gpio.h"
#include "stm32f0xx_rcc.h"

#define BLUE_BUTTON_PORT	GPIOA
#define BLUE_BUTTON_PIN     GPIO_Pin_0

uint32_t button_Init(void);
uint32_t button_Status(void);
#endif /* BUTTON_H_ */