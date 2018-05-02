

// #include "stm32f0xx_conf.h"

// void SysTick_Handler(void) {
//   static uint16_t tick = 0;

//   switch (tick++) {
//   	case 100:
//   		tick = 0;
//   		GPIOC->ODR ^= (1 << 8);
//   		break;
//   }
// }

// int main(void)
// {

	// RCC->AHBENR |= RCC_AHBENR_GPIOCEN; 	// enable the clock to GPIOC
	// 					//(RM0091 lists this as IOPCEN, not GPIOCEN)

	// GPIOC->MODER = (1 << 16);

	// SysTick_Config(SystemCoreClock/100);

	// while(1);

// }

#include "timer.h"
#include "led.h"
//#include "usart1.h"

// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
#define BLINK_ON_TICKS  (TIMER_FREQUENCY_HZ * 3 / 4)
#define BLINK_OFF_TICKS (TIMER_FREQUENCY_HZ - BLINK_ON_TICKS)

// ----- main() ---------------------------------------------------------------

int
main(int argc, char* argv[])
{
	led_Init(BLUE);
	led_Init(GREEN);
	led_Init(RED);
	led_Init(ORANGE);
//	usart1_Init();
	// Infinite loop
	timer_start();

//	uint8_t hi[] = "hello";
//	usart1_Tx(hi, sizeof(hi));

	while (1)
	{
		led_ON(ORANGE);
		led_ON(GREEN);
		led_OFF(BLUE);
		led_OFF(RED);
		timer_sleep(BLINK_ON_TICKS);
		led_OFF(ORANGE);
		led_OFF(GREEN);
		led_ON(BLUE);
		led_ON(RED);
		timer_sleep(BLINK_OFF_TICKS);
	}
	// Infinite loop, never return.
}
