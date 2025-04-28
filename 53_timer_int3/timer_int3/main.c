//* optional more complex TIMER1 interrupt example - main.c *
#include "bios_timer_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <stdint.h>
#include <avr/interrupt.h>


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define MAX_MODES         6
#define MAX_SEQU          5

#define INT_FRQ        1000
// the FRQ above is in Hz, the timing below is in 1/FRQ units
#define KEY_DEBOUNCE    100
#define KEY_LONG       1000

#define KEY_MODE       B_K4


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------

// static variables - global variables hidden in a file
// Variables shared by blinking and button - set in button and used in blinking
static uint8_t mode = 0;    // which pattern is used
static uint8_t next = 1;    // which LED is to be used starting with the next cycle


void interrupt_functionality_blinking(void)
{
    static const int16_t blinkdata[MAX_MODES][MAX_SEQU] =
        {   {  500,  500,  -1,   0,  0 },
            {    0, 1000,  -1,   0,  0 },
            {  200,  800,  -1,   0,  0 },
            {  800,  200,  -1,   0,  0 },
            { 1000,    0,  -1,   0,  0 },
            {  700,  300, 300, 600, -1 }
        };
    // static variables - global variables hidden in a function
    static uint8_t phase   = 0; // which stage of a pattern is in progress (for the current LED)
    static uint8_t current = 1; // which LED is being used (which is the current LED, updated at the end of the cycle)
    static unsigned int  counter = 0;  // Count down time (by counting interrupt runs) until the end of the current phase


    if (counter==0)
    {
        if (current!=next)                      // if switch requested
        {
            leds_set( leds_get() & ~current );  // LED off before switching
            current = next;
        }

        phase++;                                // advance to the next timing for the given sequence

        if (blinkdata[mode][phase]<0) phase=0;
        // but if we reached the sequence end then roll over to the beginning of the sequence

        while (blinkdata[mode][phase]==0) phase++;  // skip any "no time" phases

        if (blinkdata[mode][phase]<0) phase=0;
        // but if we reached the sequence end then roll over to the beginning of the sequence
        // note: actually there should be no "no time" stages at the end of a cycle

        counter=blinkdata[mode][phase];         // set up the amount of time until the next event

        // now service the event

        // check if the phase number is even or odd
        if (phase &0x01 /* ==1 */ )             // checking if the phase is odd or even
            leds_set( leds_get() & ~current );  // LED off
        else
            leds_set( leds_get() | current );   // LED on
    }
    else
    {
        counter--;
    }

}


void interrupt_functionality_button(void)
{
    // static variables - global variables hidden in a function
    // button related
    static unsigned int  time_K4_0_pressed  = 0; // How long the button has been pressed
    static unsigned int  time_K4_0_released = 0; // How long the button has been released

    // handle timing of one button
    if ( keys_get() & KEY_MODE )
    {                                           // the button is pressed
        if (time_K4_0_released>KEY_DEBOUNCE)    // check if pressed after released for at least 100ms
        {
            time_K4_0_released = 0;
        }

        if (time_K4_0_pressed<0xffff) time_K4_0_pressed++;  // increment only if still within range
    }
    else
    {                                           // the button is released
        if (time_K4_0_pressed>KEY_DEBOUNCE)     // check if released after pressed for at least 100ms
        {

            if (time_K4_0_pressed>KEY_LONG)     // long press, longer than 600ms
            {
                // ** INSERT CODE FOR ACTION WHEN RELEASED AFTER LONG PRESS **
                next = (next<<1) & 0b00001111; // switch among 4 LEDs
                if (next==0)
                    next=1;
            }
            else                                // short press (but longer than 100ms)
            {
                // ** INSERT CODE FOR ACTION WHEN RELEASED AFTER SHORT PRESS **
                mode++;
                if (mode>=MAX_MODES) mode=0;
            }


            time_K4_0_pressed = 0;              // reset the pressed value so that we handle pressed only once
        }

        if (time_K4_0_released<0xffff) time_K4_0_released++;  // increment only if still within range
    }
}



void MyTimerFN (void)
{
    // IMPORTANT: Make sure that time needed to do this entire function is (much) shorter
    // than time period between the interrupt is called.
    // If needed, use debugger on simulator to measure the time.
    interrupt_functionality_button();
    interrupt_functionality_blinking();
}



int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FRQ, &MyTimerFN, timer_prescale_64 );

    sei();

    while(1)
    {
    }

    return(0);
}
