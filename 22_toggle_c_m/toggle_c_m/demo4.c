//* bit manipulaiton demos *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

#define LED_ALIVE (1<<5)

int main(void)
{
    leds_init();
    keys_init();

    uint8_t leds   = 1;
    uint8_t toggle = 0;
    // leds and toggle are operated independently
    // and are merged just before outputting
    while(1)
    {
        toggle = toggle ^ LED_ALIVE;
        leds_set(leds | toggle);
        leds = leds >> 1;
        if ( leds == 0 )
        {
            leds = 0b00010000;
        }
        delay(100);
    }
    return(0);
}
