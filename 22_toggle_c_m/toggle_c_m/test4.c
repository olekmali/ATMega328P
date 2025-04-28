//* blink and toggle in c - test 4 - toggling finally works *
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

        // replace 0b00000001 with a different bit set, e.g., 0b00000010, 0b00000011
        // what happens then?
        if ( ( (buttons_old ^ buttons_current)& 0b00000001) != 0 )
        {
            if ( (buttons_current & 0b00000001) !=0 ) // toggle on press but not again on release
            {
                leds_current = leds_current ^ 0b00000001;
            }
        }

        buttons_old = buttons_current;

        leds_set(leds_current);
        _delay_ms(100);
    }
    return(0);
}
