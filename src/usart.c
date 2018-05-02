#include "usart.h"
/* *************** Constant / macro definitions ( #define ) *******************/
/* ********************* Type definitions ( typedef ) *************************/
/* *********************** Global data definitions ****************************/
/* **************** Global constant definitions ( const ) *********************/
/* ***************** Modul global data segment ( static ) *********************/
static uint16_t index = 0U;
/* *************** Modul global constants ( static const ) ********************/
/* **************** Local func/proc prototypes ( static ) *********************/
/******************************************************************************/
/**
* void usart_Init(tBSPType)
* @brief Configure USART1 according to board type and initialze variables.
*
*******************************************************************************/
uint32_t usart_Init(void)
{
    USART_InitTypeDef   USART_InitStructure;
    GPIO_InitTypeDef    GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_1);
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_1);
    //Configure USART2 pins: Rx (PA2) and Tx (PA3)
    GPIO_InitStructure.GPIO_Pin = USART_TX_PIN | USART_RX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Configure USART2 setting: ----------------------------
    USART_InitStructure.USART_BaudRate = 115200U;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_Cmd(USART1,ENABLE);
    return 0U;
}
/******************************************************************************/
/**
* uint32_t usart_Send(uint8_t* pTxData, uint16_t size)
* @brief Implement usart1 send.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
*
*******************************************************************************/
uint32_t usart_Send(uint8_t* pTxData, uint16_t size)
{
	while(size > 0)
	{
		size--;
		while((USART1->ISR & USART_ISR_TXE) == 0);
		USART1->TDR = pTxData[size];
	}
    return 0U;
}

/******************************************************************************/
/**
* void Usart1Reset(void)
*
* @brief Reset receive pointer index
*
* @returns    none
*
*******************************************************************************/
void Usart1Reset(void)
{
	index = 0;
}
/******************************************************************************/
/**
* uint32_t usart_Reset(void)
*
* @brief Read from UART. It will retry 3 times in case of failure.
*
* @param[out] pRxData pointer to 68 bytes data
* @param[in]  size number of bytes
* @returns    eFunction_Ok if successful
*             or
*             eFunction_Error if data losts.
*             or
*             eFunction_Timeout if an timeout error occurs.
*
*******************************************************************************/
uint32_t usart_Recv(uint8_t* pRxData, uint16_t size)
{
	uint32_t retVal = 1U;
	
	if(USART1->ISR & USART_ISR_RXNE)
	{
		pRxData[index] = USART1->RDR;
		index++;
	}
	
	if(index >= size)
	{
		index = 0;
		retVal = 0U;
	}
  return retVal;
}