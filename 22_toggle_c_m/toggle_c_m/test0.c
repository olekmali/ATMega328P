//* blink and toggle in c - test 0 - check if buttons and LEDs work *
#include "bios_leds.h"
#include "bios_keys.h"

int main(void)
{
    leds_init();
    keys_init();

    while(1)
    {
        leds_set( keys_get() );
    }

    return(0);
}
