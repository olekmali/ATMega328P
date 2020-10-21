//* bios_keys.c *
#include "bios_keys.h"
#include <avr/io.h>

void keys_init (void)
{
    // initialize BUTTON input pins on upper 4 bits of Port D
    DDRD  = DDRD  & 0x0F;       // affect DDR only the 4 upper pins - set to in/0
    PORTD = PORTD | 0xF0;       // affect pull up resistors only the 4 upper pins
}

void keys_shutdown (void)
{
    // to be safe and low power set the upper nibble back to input and shut off pull up resistors
    DDRD &= 0x0F;
    PORTD&= 0x0F;
}

uint8_t keys_get (void)
{
    return( ((~PIND)>>4) & 0x0F );
}
