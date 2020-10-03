//* bios_leds.c *
#include "bios_leds.h"
#include <avr/io.h>

void leds_init (void)
{
    // initialize LED output pins on lower 6 bits of Port B
    DDRB  = DDRB  | 0b00111111;   // affect DDR only the lower pins - set to push-pull out/1
    // PORTB = PORTB | 0x0001111111;   // affect only the 6 output pins by setting them to 1
    PORTB = PORTB & 0b11000000;   // affect only the 6 output pins by setting them to 0
}

void leds_shutdown (void)
{
    DDRB  &= 0b11000000;
    PORTB &= 0b11000000;
}

uint8_t leds_get (void)
{
    return(PINB & 0b00111111);
}

void leds_set ( uint8_t number )
{
    PORTB = (PORTB & 0b11000000) | (number & 0b00111111);
}
