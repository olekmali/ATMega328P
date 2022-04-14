//* bit manipulaiton demos *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

int main(void)
{
    leds_init();
    keys_init();

    uint8_t leds = 1;
    while(1)
    {
        leds_set(leds);
        leds = leds >> 1;
        if ( leds == 0 )
        {
            leds = 0b00010000;
        }
        delay(100);
    }
    return(0);
}
