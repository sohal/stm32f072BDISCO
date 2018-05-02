/*
 * button.c
 *
 *  Created on: Jan 19, 2018
 *      Author: p
 */

#include "button.h"
uint32_t button_Init(void)
{
	uint32_t retVal = 0;
	GPIO_InitTypeDef button_GPIOTypedef;
    button_GPIOTypedef.GPIO_Pin = BLUE_BUTTON_PIN;

	button_GPIOTypedef.GPIO_Mode = GPIO_Mode_IN;
	button_GPIOTypedef.GPIO_OType = GPIO_OType_PP;
	button_GPIOTypedef.GPIO_Speed = GPIO_Speed_50MHz;
	button_GPIOTypedef.GPIO_PuPd = GPIO_PuPd_NOPULL;

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	GPIO_Init(BLUE_BUTTON_PORT, &button_GPIOTypedef);

	return(retVal);
}
uint32_t button_Status(void)
{
	uint32_t retVal = 0;
	retVal = (uint32_t)GPIO_ReadOutputDataBit(BLUE_BUTTON_PORT, BLUE_BUTTON_PIN);
	return(retVal);
}
