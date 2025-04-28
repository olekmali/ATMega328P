//* blink and toggle in c - test 7 - looking at a button without an oscilloscope *
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
        uint8_t buttons_current = keys_get();

        // reset the button press count
        if ( (buttons_current & 0b00000010) !=0 ) {
            leds_current = 0;
        }

        // does it always toggle only once on press?
        if ( ( (buttons_old ^ buttons_current)& 0b00000001) != 0 )
        {
            if ( (buttons_current & 0b00000001) !=0 )
            {
                leds_current++;
            }
        }

        buttons_old = buttons_current;

        leds_set(leds_current);
        // _delay_ms(1); // <- try minimum time delay when 
        // no multiple button press are recorded over at least 10 attempts 
        // and then double that time for safety
    }
    return(0);
}
