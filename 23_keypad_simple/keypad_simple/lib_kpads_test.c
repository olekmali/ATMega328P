//* lib_kpad_test.c *
#include "lib_kpads_test.h"
#include "bios_leds.h"
#include "bios_kpads.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

int test0_getkey_value(void)
{
    uint8_t toggle = 0;
    leds_init();


    while(1)
    {
        leds_set( getkey() | toggle );
        toggle = toggle ^ 0b00010000;
    }
    return(0);
}

int test1_count_up(void)
{
    leds_init();

    uint8_t i=0;
    while(1)
    {
        i++;
        leds_set(i);
        _delay_ms(100);
    }
    return(0);
}

int test2_shift(void)
{
    leds_init();

    uint8_t i=1;
    while(1)
    {
        i = i<<1;
        if ( i > 16 ) i = 1; // make it use only 5 bits
        leds_set(i);
        _delay_ms(100);
    }
    return(0);
}

int test8_keypressed_status(void)
{
    leds_init();

    while(1)
    {
        leds_set( keypressed() );

    }
    return(0);
}
