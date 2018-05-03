/******************************************************************************/
/**
* @file Usart1.h
* @brief Implement usart1
*
*******************************************************************************/
#ifndef USART_H
#define USART_H

/* ***************** Header / include files ( #include ) **********************/
#include "stm32f0xx.h"
#include "common.h"
#include "gpio.h"
#include "bsp.h"
/* *************** Constant / macro definitions ( #define ) *******************/
#define __DIV(__PCLK, __BAUD)       ((__PCLK*25)/(4*__BAUD))
#define __DIVMANT(__PCLK, __BAUD)   (__DIV(__PCLK, __BAUD)/100)
#define __DIVFRAQ(__PCLK, __BAUD)   (((__DIV(__PCLK, __BAUD) - (__DIVMANT(__PCLK, __BAUD) * 100)) * 16 + 50) / 100)
#define __USART_BRR(__PCLK, __BAUD) ((__DIVMANT(__PCLK, __BAUD) << 4)|(__DIVFRAQ(__PCLK, __BAUD) & 0x0F))

/* ********************* Type definitions ( typedef ) *************************/
/* ***************** Global data declarations ( extern ) **********************/
/* ***** External parameter / constant declarations ( extern const ) **********/
/* ********************** Global func/proc prototypes *************************/
void usartInit(tBSPType BSPType);
void usartSend(uint8_t *pTxData, uint16_t size);
void usartReset(void);
eFUNCTION_RETURN usartRecv(uint8_t *pRxData, uint16_t size);
void usartDeInit(void);
#endif
