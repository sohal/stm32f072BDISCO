#include "timer.h"
#include "led.h"
#include "usart.h"
#include "button.h"
// ----- Timing definitions -------------------------------------------------

// Keep the LED on for 2/3 of a second.
static uint32_t BLINK_ON_TICKS 	= (TIMER_FREQUENCY_HZ * 3 / 4);
static uint32_t BLINK_OFF_TICKS = (TIMER_FREQUENCY_HZ * 1 / 4);

// ----- main() ---------------------------------------------------------------

int
main(int argc, char* argv[])
{
	led_Init(BLUE);
	led_Init(GREEN);
	led_Init(RED);
	led_Init(ORANGE);
	usart_Init();
	timer_start();
	button_Init();

	uint8_t hi[] = "hello";
	usart_Send(hi, sizeof(hi));

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
		if(button_Status())
		{
			uint32_t temp = BLINK_OFF_TICKS;
			BLINK_OFF_TICKS = BLINK_ON_TICKS;
			BLINK_ON_TICKS = temp;
			hi[0] = 's';
			hi[1] = '0';
			hi[2] = 'h';
			hi[3] = 'a';
			hi[4] = 'l';
			usart_Send(hi, sizeof(hi));
		}
	}
	// Infinite loop, never return.
}
