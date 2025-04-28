//* blink and toggle in c - test 3 - are we toggling LED with a button? *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

int main(void)
{
    leds_init();
    keys_init();

    uint8_t buttons_old;
    uint8_t leds_current = 0;
    buttons_old = keys_get();
    while(1)
    {
        leds_current = leds_current ^ 0b00100000;

        uint8_t buttons_current = keys_get();

        // what is wrong with toggling?
        if ( ( (buttons_old ^ buttons_current)& 0b00000001) != 0 )
        {
            leds_current = leds_current ^ 0b00000001;
        }

        buttons_old = buttons_current;

        leds_set(leds_current);
        _delay_ms(100);
    }
    return(0);
}
