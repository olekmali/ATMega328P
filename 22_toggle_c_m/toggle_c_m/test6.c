//* blink and toggle in c - test 6 - is delay really needed? *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

int main()
{
    leds_init();
    keys_init();

    uint8_t buttons_old;
    uint8_t leds_current = 0;
    buttons_old = keys_get();
    while(1)
    {
        // leds_current = leds_current ^ 0b00100000; <- blinking is too fast for human eye nowS

        uint8_t buttons_current = keys_get();

        // does it always toggle only once on press?
        if ( ( (buttons_old ^ buttons_current)& 0b00000001) != 0 )
        {
            if ( (buttons_current & 0b00000001) !=0 )
            {
                leds_current = leds_current ^ 0b00000001;
            }
        }

        buttons_old = buttons_current;

        leds_set(leds_current);
        // _delay_ms(100); <- removed delay
    }
    return(0);
}
