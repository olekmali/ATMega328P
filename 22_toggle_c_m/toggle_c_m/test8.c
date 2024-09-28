//* blink and toggle in c - test 8 - exercises with bit shifting *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

int main()
{
    leds_init();
    keys_init();

    uint8_t byte  = 0b00000001;
    uint8_t blink = 0b00010000;
    uint8_t count = 0U;
    while(1) {
        byte = byte << 1;
        if ( (byte & 0b001111) == 0  ) byte = 0b00000001;
        //   (                ) are necessary because & is less important than !=
        // if the bit left the area of shifting then restart
        if (count > 0 ) {
            count--;
        } else { // this counts 5 4 3 2 1 0 5 4 3 2 1 0 5 ...
            count = 5U;
            blink = blink ^ 0b00010000;
        }
         byte = (byte & 0b00001111) | blink;
         //                   ^^^^ area for shifting
        leds_set(byte);
        _delay_ms(200);
    }
    return(0);
}
