//* blink and toggle in c - test 10 - exercises with bit shifting *
#include <stdint.h>
#include "bios_leds.h"
#include "bios_keys.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

int main(void)
{
    leds_init();
    keys_init();

    uint8_t byte  = 0b00000000;
    while(1) {
//     Total baseline code size without byte manipulation 206/206 Bytes

//     The alternative code line below takes either 5 or 7 clock cycles, +18/+14 Bytes
       byte = byte << 1;
        if ( (byte & 0b00100000) == 0 ) {
            byte = ( byte | 0b00000001 ) & 0b00011111;
        } else {
            byte = byte & 0b00011111;
        }

//     The alternative code line below takes always 17 clock cycles, TL +38/+24 Bytes
//     byte = ( (byte << 1) & 0b00011110 ) | ( ( (~byte) >> 4) & 0b00000001);

        leds_set(byte);
        _delay_ms(200);
    }
    return(0);
}
