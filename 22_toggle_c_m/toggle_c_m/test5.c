//* blink and toggle in c - test 5 - coded more professionally *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

// this is coded more professionally without magics numbers in the code
#define B_ONE 0b00000001
#define L_ONE 0b00000001
#define L_ALIVE 0b00100000


int main()
{
    leds_init();
    keys_init();

    uint8_t buttons_old;
    uint8_t leds_current = 0;
    buttons_old = keys_get();
    while(1)
    {
        leds_current = leds_current ^ L_ALIVE;

        uint8_t buttons_current = keys_get();

        if ( ( (buttons_old ^ buttons_current) & B_ONE) != 0  &&  ( (buttons_current & B_ONE) !=0 ))
        {
            leds_current = leds_current ^ L_ONE;
        }

        buttons_old = buttons_current;

        leds_set(leds_current);
        _delay_ms(100);
    }
    return(0);
}
