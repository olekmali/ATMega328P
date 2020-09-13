//* blink and toggle in c - main.c for test 2 *
#include <stdint.h>
#include "bios.h"
#include "library.h"

// this test allows you to do the final tweak of the delay function
int main()
{
    uint8_t led_current = 0;
    init_digital_io();

    while(1)
    {
        led_current = toggle_bit(led_current, 0x0F);
        set_leds(led_current);
        delay(10000); // should be 10 seconds on and 10 seconds off
        // (1) measure the time period of ON or OFF on the board in release mode
        // (2) use proportion to adjust the inner loop counter in delay(.)
    }

    return(0);
}
