//* blink and toggle in c - main.c for test 1 *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

// this test partially tests toggle_bit(.) function
int main(void)
{
    leds_init();
    keys_init();

    uint8_t led_current = 0;
    while(1)
    {
        // replace 7 with 0,... in toggle_bit(.) to test more thoroughly
        led_current = toggle_bit(led_current, 7);
        leds_set(led_current);
        delay(100);
    }

    return(0);
}
