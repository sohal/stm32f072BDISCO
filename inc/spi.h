/*
 * spi.h
 *
 *  Created on: May 02, 2018
 *      Author: p
 */

#ifndef SPI_H_
#define SPI_H_
#include <stdint.h>
#include "stm32f0xx_spi.h"      /** For SPI */
#include "stm32f0xx_gpio.h"     /** For MOSI(PA7), MISO(PA6), CLK(PA5) and CS(PA4) */
#include "stm32f0xx_rcc.h"
#define SPI1_PORT           GPIOA
#define SPI1_NSS            GPIO_Pin_4
#define SPI1_CLK            GPIO_Pin_5
#define SPI1_MISO           GPIO_Pin_6
#define SPI1_MOSI           GPIO_Pin_7

uint32_t spi_Init(void);
uint32_t spi_Send(uint8_t* pTxData, uint16_t size);
void     spi_Reset(void);
uint32_t spi_Recv(uint8_t* pRxData, uint16_t size);
#endif /* SPI_H_ */