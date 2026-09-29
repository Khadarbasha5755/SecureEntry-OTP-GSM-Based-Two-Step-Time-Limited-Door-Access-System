//delay.h

#include "types.h"

/* Select hardware timing delays. */
#define HARDWARE

/* Generate delay in microseconds. */
void delay_us(u32);

/* Generate delay in milliseconds. */
void delay_ms(u32);

/* Generate delay in seconds. */
void delay_s(u32);
