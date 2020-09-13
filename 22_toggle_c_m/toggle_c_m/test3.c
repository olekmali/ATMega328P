//* blink and toggle in c - main.c for test 3 *
#include <stdint.h>
#include "bios.h"
#include "library.h"

// this test allows you to partially test the check_buttons function
int main()
{
    uint8_t buttons_current, buttons_old;
    uint8_t led_current = 0;
    init_digital_io();
    buttons_old = get_buttons();
    while(1)
    {
        // led_current = toggle_bit(led_current, 7);

        buttons_current = get_buttons();

         // replace 0 with 1,... in check_buttons(.) to test more thoroughly
        if ( 0!=check_button(buttons_old, buttons_current, 0) )
            led_current = toggle_bit(led_current, 0);

        buttons_old = buttons_current;

        set_leds(led_current);
        delay(100);
    }
    return(0);
}
