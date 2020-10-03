//* blink and toggle in c - main.c for test 0 *
#include "bios_leds.h"
#include "bios_keys.h"
// #include "library.h"

// This test should work out of the box without your library
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
