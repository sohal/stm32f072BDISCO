#include "bsp.h"

static tBSPStruct gIF;
static void TorqueSensorCoreClockInit(void);
static void TorqueSensorCoreClockDeInit(void);

tBSPStruct* BSP_Init(void)
{
	gIF.pInit 						= NULL;
	gIF.pDeInit 					= NULL;
	gIF.pSend 						= NULL;
	gIF.pReset 						= NULL;
	gIF.BSP_Type 					= BSP_UnKnown;
	gIF.BootTimeoutTicks	= BootTIMEOUT;
	gIF.AppStartTicks		  = BootTIMEOUT - 100000UL;
	gIF.CommDoneTicks 		= 10000UL;
	gIF.TwoBytesTicks			= 300UL;
	uint32_t temp_u32 		= 0UL;

	temp_u32 = DBGMCU->IDCODE;
	
	if((temp_u32 & DBGMCU_IDCODE_DEV_ID) != DBGMCU_ID_F03x)
	{
		/* Make BSP config for F04x, Interface is always CAN */
		gIF.BSP_Type = BSP_STM32F042;
	}else
	{
		/* Interface can be SPI or USART for F03x family so more investigation necessary */
	
		RCC->AHBENR |= RCC_AHBENR_GPIOAEN;
		/* Input pin 6 is made active */
		BSP_CHECK_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_CHECK_PIN_6 * 2));
		BSP_CHECK_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_CHECK_PIN_6 * 2));
		/* Input pin 6 is made active */
		BSP_CHECK_PORT->MODER &= ~(GPIO_MODER_MODER0 << (BSP_CHECK_PIN_7 * 2));
		BSP_CHECK_PORT->PUPDR &= ~(GPIO_PUPDR_PUPDR0 << (BSP_CHECK_PIN_7 * 2));
		
		BSP_CHECK_PORT->ODR   &= ~((MASK_BIT_6) | (MASK_BIT_7));
		
		temp_u32 = BSP_CHECK_PORT->IDR;
		
		if(temp_u32 & (MASK_BIT_6))
		{
			/* Pin 6 is high only for Watchdog */
			gIF.BSP_Type = BSP_ExtWatchdog;
		}else
		{
			if(temp_u32 & (MASK_BIT_7))
			{
				/* Pin 7 is high only for Pilot */
				gIF.BSP_Type = BSP_Pilot;
			}else
			{
				/* This shall be a Torque sensor */
				gIF.BSP_Type = BSP_TorqueSensor;
			}
		}
	}
	
#if defined (SELECT_TORQUE)
	gIF.BSP_Type = BSP_TorqueSensor;
	#warning Torque Sensor is selected 
#elif defined (SELECT_PILOT)
	gIF.BSP_Type = BSP_Pilot;
	#warning Pilot is selected
#elif defined (SELECT_CAN)
	gIF.BSP_Type = BSP_STM32F042;
	#warning CAN bus selected
#else
	#error Select a valid board type
#endif
	switch(gIF.BSP_Type)
	{
		case BSP_Pilot:
			gIF.pInit 	= &usartInit;
			gIF.pRecv 	= &usartRecv;
			gIF.pSend 	= &usartSend;
			gIF.pReset 	= &usartReset;
			gIF.pDeInit = &usartDeInit;
			break;
		
		case BSP_TorqueSensor:
			gIF.pInit 	= &usartInit;
			gIF.pRecv 	= &usartRecv;
			gIF.pSend 	= &usartSend;
			gIF.pReset 	= &usartReset;
			gIF.pDeInit = &usartDeInit;
			TorqueSensorCoreClockInit();
			break;
		
		case BSP_ExtWatchdog:
			gIF.pInit 	= NULL;
			gIF.pRecv 	= NULL;
			gIF.pSend 	= NULL;
			gIF.pDeInit = &usartDeInit;
			break;
		
		case BSP_NucleoF0x:
			gIF.pInit 	= &usartInit;
			gIF.pRecv 	= &usartRecv;
			gIF.pSend 	= &usartSend;
			gIF.pReset 	= &usartReset;
			gIF.pDeInit = &usartDeInit;
			break;
		
		case BSP_STM32F042:
			gIF.pInit		= &CanInit;
			gIF.pRecv 	= &CanRecv;
			gIF.pSend 	= &CanSend;
			gIF.pReset 	= &CanReset;
			gIF.pDeInit = &CanDeInit;
			break;
		
		default:
			gIF.pInit = NULL;
			break;
	}
  
	/* Let's update the global SystemCoreClock variable just in case the system
	 * frequency has changed. Mandatory for calculations of delay for bootloader
	 * timeouts that are solely dependent on system ticks
	 */
	SystemCoreClockUpdate();
	/* Now calculate by what factor has the system changed it's core clock */
	temp_u32 = ( SystemCoreClock / BSP_ALLBOARD_HSI_FREQUENCY );
	
	gIF.AppStartTicks 		*= temp_u32;
	gIF.CommDoneTicks 		*= temp_u32;
	gIF.BootTimeoutTicks  *= temp_u32;
	gIF.TwoBytesTicks			*= temp_u32;
	
	gIF.pInit(gIF.BSP_Type);
	
	FlashInit(gIF.BSP_Type);
	
	return(&gIF);
}

void BSP_DeInit(void)
{
	if(gIF.BSP_Type == BSP_TorqueSensor)
	{
		TorqueSensorCoreClockDeInit();
	}
	//gif.pDeInit();
	
}

void TorqueSensorCoreClockInit(void)
{
    RCC->CR |= ((uint32_t)RCC_CR_HSION);                       /* Enable HSI */
    while ((RCC->CR & RCC_CR_HSIRDY) == 0);                  /* Wait for HSI Ready */

    RCC->CFGR = RCC_CFGR_SW_HSI;                             /* HSI is system clock */
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI);  /* Wait for HSI used as system clock */

    FLASH->ACR  = FLASH_ACR_PRFTBE;                          /* Enable Prefetch Buffer */
    FLASH->ACR |= FLASH_ACR_LATENCY;                         /* Flash 1 wait state */

    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;                         /* HCLK = SYSCLK */
    RCC->CFGR |= RCC_CFGR_PPRE_DIV1;                         /* PCLK = HCLK */

    RCC->CR &= ~RCC_CR_PLLON;                                /* Disable PLL */

    /*  PLL configuration:  = HSI/2 * 8 = 32 MHz */
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    RCC->CFGR |=  (RCC_CFGR_PLLSRC_HSI_Div2 | RCC_CFGR_PLLMULL16);  /* 16 for 64Mhz */

    RCC->CR |= RCC_CR_PLLON;                                 /* Enable PLL */
    while((RCC->CR & RCC_CR_PLLRDY) == 0) __NOP();           /* Wait till PLL is ready */

    RCC->CFGR &= ~RCC_CFGR_SW;                               /* Select PLL as system clock source */
    RCC->CFGR |=  RCC_CFGR_SW_PLL;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);  /* Wait till PLL is system clock src */
}

void TorqueSensorCoreClockDeInit(void)
{
	//ToDo
	//SystemClockUpdate();
}
