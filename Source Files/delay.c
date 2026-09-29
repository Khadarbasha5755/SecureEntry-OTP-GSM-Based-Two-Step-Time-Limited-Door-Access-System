//delay.c

#include "types.h"


/* MICROSECOND DELAY */
void delay_us(u32 tdly)
{
    s32 i,j;

    /* Repeat the loop for the required number of microseconds. */
    for(i=0;i<tdly;i++)
    {
        /* Inner loop creates the required time delay. */
        for(j=0;j<12;j++);
    }

    /* Approximately 5 clock cycles are taken per loop. */
    /* 12 * 5 = 60 clock cycles. */
}


/* MILLISECOND DELAY */
void delay_ms(u32 tdly)
{
    s32 i,j;

    /* Repeat the loop for the required number of milliseconds. */
    for(i=0;i<tdly;i++)
    {
        /* Inner loop creates the required time delay. */
        for(j=0;j<12000;j++);
    }

    /* Approximately 5 clock cycles are taken per loop. */
    /* 12000 * 5 = 60000 clock cycles. */
}


/* SECOND DELAY */
void delay_s(u32 tdly)
{
    s32 i,j;

    /* Repeat the loop for the required number of seconds. */
    for(i=0;i<tdly;i++)
    {
        /* Inner loop creates the required time delay. */
        for(j=0;j<12000000;j++);
    }

    /* Approximately 5 clock cycles are taken per loop. */
    /* 12000000 * 5 = 60000000 clock cycles. */
}
