//* blink and toggle in c - main.c for test 4 *
#include <stdint.h>
#include "bios.h"
#include "library.h"

// constant values
#define ALIVE    5

#define map_size 4
// we use only 3 buttons because we have only 4 LEDs
// and we use the 6th LED (5 when counting from 0) for "alive" blinking

#define B0       0
#define B1       1
#define B2       2
#define B3       3

#define L0       0
#define L1       1
#define L2       2
#define L3       3


int main(void)
{
    // constant data
    // Note: expressions (1<<const) are known at the compilation time and are replaced by resulting constant values - no code!
    /* static */ const uint8_t map_keys[map_size] = { 1<<B0, 1<<B1, 1<<B2, 1<<B3 };
    /* static */ const uint8_t map_leds[map_size] = { 1<<L0, 1<<L1, 1<<L2, 1<<L3 };

    // variables
    uint8_t buttons_current, buttons_old;
    uint8_t led_current = 1<<ALIVE;  // LED are off except for the "alive" LED

    // initialization code
    init_digital_io();
    buttons_old = get_buttons();

    // main program super loop
    while(1)
    {
        led_current = toggle_bit(led_current, (1<<ALIVE) );

        buttons_current = get_buttons();

        for(uint8_t i=0; i<map_size; i++) {
            if ( 0!=check_button(buttons_old, buttons_current, map_keys[i] ) )
                led_current = toggle_bit(led_current, map_leds[i]);
        }
        buttons_old = buttons_current;

        set_leds(led_current);
        delay(100);
    }
    return(0);
}
