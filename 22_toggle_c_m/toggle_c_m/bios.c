//* blink and toggle in c - bios.c *
#include "bios.h"

// let us keep the hardware information as enclosed as possible
#include <avr/io.h>

void init_digital_io()
{
    // initialize LED output pins on lower 6 bits of Port B
    DDRB  = DDRB  | 0b00111111;     // affect DDR only the lower pins - set to push-pull out/1
    // PORTB = PORTB | 0x0001111111;   // affect only the 6 output pins by setting them to 1
    PORTB = PORTB & 0b11000000;     // affect only the 6 output pins by setting them to 0

    // initialize BUTTON input pins on upper 4 bits of Port D
    DDRD  = DDRD  & 0x0F;       // affect DDR only the 4 upper pins - set to in/0
    PORTD = PORTD | 0xF0;       // affect pull up resistors only the 4 upper pins
}


uint8_t get_buttons()
{
    return( ((~PIND)>>4) & 0x0F );
}

void set_leds(uint8_t new_status)
{
    PORTB = (PORTB & 0b11000000) | (new_status & 0b00111111);
}
