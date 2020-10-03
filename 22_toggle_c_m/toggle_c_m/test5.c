//* blink and toggle in c with key mapping - main.c for test 5 *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"
#include "library.h"

// constant values
#define MAP_SIZE  4
// and we use the 6th LED (5 when counting from 0) for "alive" blinking
#define LED_ALIVE 5

int main(void)
{
    // initialization code
    leds_init();
    keys_init();

    // constant data
    // Note: expressions (1<<const) are known at the compilation time and are replaced by resulting constant values - no code!
    /* static */ const uint8_t map_keys[MAP_SIZE] = { B_K4, B_K5, B_K6, B_K7 };
    /* static */ const uint8_t map_leds[MAP_SIZE] = { B_L0, B_L1, B_L2, B_L3 };

    // variables
    uint8_t led_current = 1<<LED_ALIVE;  // LED are off except for the "alive" LED
    uint8_t buttons_current = keys_get();
    // main program super loop
    while(1)
    {
        uint8_t buttons_old = buttons_current;

        led_current = toggle_bit(led_current, (1<<LED_ALIVE) );

        buttons_current = keys_get();

        for(uint8_t i=0; i<MAP_SIZE; i++) {
            if ( 0!=check_button(buttons_old, buttons_current, map_keys[i] ) )
                led_current = toggle_bit(led_current, map_leds[i]);
        }

        leds_set(led_current);
        delay(100);
    }
    return(0);
}
