//* blink and toggle in c - test 2 - demonstration of bitwise shift function *
#include <stdint.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

#include "bios_leds.h"
#include "bios_keys.h"

// this test allows you to do the final tweak of the delay function
int main(void)
{
    leds_init();
    keys_init();

    uint8_t leds_current = 0;
    while(1)
    {
        // can you change the direction of scrolling light?
        leds_current = leds_current << 1;
        // experiment with different number of 0s followed by 1s (still total 8 bits)
        // what happens if you put a 0 bit between 1s? why?
        if ( (leds_current & 0b00011111) == 0 )
        {
            leds_current = 1;
        }
        leds_set(leds_current);
        _delay_ms(100);
    }

    return(0);
}
