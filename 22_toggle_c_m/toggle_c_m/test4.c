//* blink and toggle in c - main.c for test 4 *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

// this test toggles four LEDs using four buttons
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
        led_current = toggle_bit(led_current, check_button(buttons_old, buttons_current, 0b00001111) );

        buttons_old = buttons_current;

        leds_set(led_current);
        delay(100);
    }
    return(0);
}
