#include "spi.h"
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
* void spi_Init(tBSPType)
* @brief Configure SPI according to board type and initialze variables.
*
*******************************************************************************/
uint32_t spi_Init(void)
{
    SPI_InitTypeDef     SPIDevice;
    GPIO_InitTypeDef    GPIO_InitStructure;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    //Configure SPI1 pins
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    /** SPI1_MISO PA6 */
    GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource6, GPIO_AF_0);
    GPIO_InitStructure.GPIO_Pin = SPI1_MISO;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /** SPI1_MOSI PA7 & SPI1_CLK PA5 */
    GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource5, GPIO_AF_0);
    GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource7, GPIO_AF_0);
    GPIO_InitStructure.GPIO_Pin = SPI1_MOSI | SPI1_CLK;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /** SPI1_NSS PA4 */
    GPIO_PinAFConfig(SPI1_PORT, GPIO_PinSource4, GPIO_AF_0);
    GPIO_InitStructure.GPIO_Pin = SPI1_NSS;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //Configure SPI1 setting: ----------------------------
    /* https://visualgdb.com/tutorials/arm/stm32/spi/2boards/ */
    SPIDevice.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
    SPIDevice.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPIDevice.SPI_CPOL = SPI_CPOL_Low;
    SPIDevice.SPI_CPHA = SPI_CPHA_2Edge;
    SPIDevice.SPI_CRCPolynomial = 0xFFFF;
    SPIDevice.SPI_DataSize = SPI_DataSize_8b;
    SPIDevice.SPI_FirstBit = SPI_FirstBit_LSB;
    SPIDevice.SPI_Mode = SPI_Mode_Slave;
    SPIDevice.SPI_NSS = SPI_NSS_Hard;

    SPI_Init(SPI1, &SPIDevice);
    SPI_Cmd(SPI1,ENABLE);
    return 0U;
}
/******************************************************************************/
/**
* uint32_t spi_Send(uint8_t* pTxData, uint16_t size)
* @brief Implement spi1 send.
*
* @param[in] pTxData pointer to the data to be transmitted
* @param[in] size number of bytes
*
*******************************************************************************/
uint32_t spi_Send(uint8_t* pTxData, uint16_t size)
{
    while(size > 0)
	{
		size--;
        while(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) != SET)
        {
            ;
        }
		SPI_SendData8(SPI1, pTxData[size]);
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
void spi_Reset(void)
{
	index = 0;
}
/******************************************************************************/
/**
* uint32_t spi_Reset(void)
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
uint32_t spi_Recv(uint8_t* pRxData, uint16_t size)
{
	uint32_t retVal = 1U;    

	if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET)
	{
		pRxData[index] = SPI_ReceiveData8(SPI1);
		index++;
	}
	
	if(index >= size)
	{
		index = 0;
		retVal = 0U;
	}
    return retVal;
}