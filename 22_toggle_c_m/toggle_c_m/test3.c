//* blink and toggle in c - main.c for test 3 *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

// this test allows you to partially test the check_buttons function
int main()
{
    leds_init();
    keys_init();

    uint8_t buttons_current, buttons_old;
    uint8_t led_current = 0;
    buttons_old = keys_get();
    while(1)
    {
        led_current = toggle_bit(led_current, 0b00100000); // tests if the loop runs

        buttons_current = keys_get();

         // replace 0!= with 0b0001, 0b0010, , 0b000 to test a particular button
        if ( 0!=check_button(buttons_old, buttons_current, 0b00001111) )
        {
            // this toggles only the 1st LED no matter which button press was detected
            led_current = toggle_bit(led_current, 1);
        }

        buttons_old = buttons_current;

        leds_set(led_current);
        delay(100);
    }
    return(0);
}
