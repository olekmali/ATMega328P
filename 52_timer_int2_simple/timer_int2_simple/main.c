//* Introductory examples for TIMER1 - interrupt semaphore concept - main.c *
#include "bios_timer_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <stdlib.h>
#include <avr/interrupt.h>

#define INT_FREQUENCY   100
#define INT_PRESCALE    timer_prescale_256

static volatile uint8_t semaphore = 0;
// use of volatile is a must or main loop will not notice the variable change
// use of static is optional (but good practice) to restrict the global visibility
// use a variable of size that can be read in one clock cycle (8bit uC => uint8_t)

void myInterruptFunctionality() {
    semaphore = 1;
}

int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();
    Timer1_initialize( INT_FREQUENCY ,myInterruptFunctionality, INT_PRESCALE);

    sei();
    while (1)
    {
        // if (semaphore!=0) { /* indicate error that the loop took too long to run */ }

        // now waiting for the interrupt to happen
        while(semaphore==0)
            ;
        semaphore=0;

        leds_set( leds_get()^0b00000001 );
    }

    return(0);
}
