//* blink and toggle in c - library.c *
#include "library.h"

void delay ( unsigned short int milliseconds )
{
    while (milliseconds>0)
    {
        volatile uint16_t counter;
        for (counter=0; counter<888; counter++) ;
        milliseconds--;
    }
}

uint8_t check_button ( uint8_t old_buttons, uint8_t new_buttons, uint8_t button_mask)
{
    return ( ( old_buttons ^ new_buttons ) & new_buttons & button_mask );
}

uint8_t toggle_bit ( uint8_t old_bits, uint8_t bit_mask)
{
    return ( old_bits ^ bit_mask );
}
