/*
 * usart.h
 *
 *  Created on: May 02, 2018
 *      Author: p
 */

#ifndef USART_H_
#define USART_H_
#include <stdint.h>
#include "stm32f0xx_usart.h" /** For USART1 */
#include "stm32f0xx_gpio.h"     /** For RxPin(PA9) and TxPin(PA10) */
#include "stm32f0xx_rcc.h"

#define USART_TX_PIN		GPIO_Pin_9
#define USART_RX_PIN	    GPIO_Pin_10

uint32_t usart_Init(void);
uint32_t usart_Send(uint8_t* pTxData, uint16_t size);
uint32_t usart_Reset(void);
uint32_t usart_Recv(uint8_t* pRxData, uint16_t size);
#endif /* USART_H_ */