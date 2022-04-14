//* blink and toggle in c - library.c *
#include "library.h"

/*
    The function implements delay using a simple idle loop strategy.
    Fine tune the delay by setting correct number of the inner loop iterations.
    Measure the delay by calling the function for a long delay time, say 5 seconds
    and compare with a stopwatch. Then use linear proportion to tweak the inner loop.
    Alternatively use the professional approach by utilizing an oscilloscope in the lab.
*/
void delay ( unsigned short int milliseconds )
{
    while (milliseconds>0)
    {
        volatile uint16_t counter;
        for (counter=0; counter<888; counter++) ;
        milliseconds--;
    }
}

/*
    old_buttons and new_buttons are the previous and current states of the buttons/switches
    button_mask is used to identify which bits are to be checked for change, and which are not
    the function was discussed in class two times, the assembler competition and the intro to BIOS API
    the button_mask is to be used to do the bit-by-bit AND before the the results are returned
*/
uint8_t check_button ( uint8_t old_buttons, uint8_t new_buttons, uint8_t button_mask)
{
    return ( ( old_buttons ^ new_buttons ) & new_buttons & button_mask );
}

/*
    Toggle the bits of old_bits that are set to 1 in bit_mask.
    Return the updated variable with the requested bits toggled.
    A simple bit-by-bit exclusive OR may do the trick.
*/
uint8_t toggle_bit ( uint8_t old_bits, uint8_t bit_mask)
{
    return ( old_bits ^ bit_mask );
}
