//* bios_leds.c *
#include "bios_leds.h"
#include <avr/io.h>

//#define F_CPU (16000000UL)
//#include <util/delay.h>

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
    // no need for delay after writing to PORTA
    // as it takes time to call the function
    return(PINB & 0b00111111);
}

void leds_set ( uint8_t number )
{
    PORTB = (PORTB & 0b11000000) | (number & 0b00111111);
}

void delay (unsigned short int milliseconds)
{
    while (milliseconds>0)
    {
        volatile uint16_t counter;
        for (counter=0; counter<888; counter++) ;
        milliseconds--;
    }
}
