//* blink and toggle in c - test 1 - toggling using XOR and use of delay function *
#include <stdint.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

#include "bios_leds.h"
#include "bios_keys.h"

int main(void)
{
    leds_init();
    keys_init();

    uint8_t leds_current = 0;
    while(1)
    {
        // replace 0b00000001 with different numbers to test the behavior of operator ^
        leds_current = leds_current ^ 0b00000001;
        leds_set(leds_current);
        _delay_ms(100);
    }

    return(0);
}
