//* testing Watchdog - main.c *
#include "bios_leds.h"
#include "bios_keys.h"
#include <avr/wdt.h>

#define F_CPU 16000000UL
#include <util/delay.h>


int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    wdt_enable(3);              // change it to 0 or 1 - less than 50ms, observe and put it back
    // now you must reset watchdog more frequently than:
    // 0 - 14.0ms
    // 1 - 28.1ms
    // 2 - 56.2ms
    // 3 - 0.11s
    // 4 - 0.22s
    // 5 - 0.45s
    // 6 - 0.9s
    // 7 - 1.8s

    while(1)
    {
        wdt_reset();                // comment this line, observe and put it back
        leds_set( leds_get()+1 );
        _delay_ms(50);              // change it to more than 110ms, observe and put it back
    }

    return(0);
}


