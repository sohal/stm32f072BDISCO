#ifndef TIMER_H_
#define TIMER_H_
#include <stdint.h>
#include "stm32f0xx.h"
// ----------------------------------------------------------------------------

#define TIMER_FREQUENCY_HZ (1000u)

typedef uint32_t timer_ticks_t;

extern volatile timer_ticks_t timer_delayCount;

extern void
timer_start (void);

extern void
timer_sleep (timer_ticks_t ticks);

// ----------------------------------------------------------------------------

#endif // TIMER_H_