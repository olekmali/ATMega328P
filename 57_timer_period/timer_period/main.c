//* testing TIMER1 interrupt - main.c *
#include "bios_timer_period.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <stdint.h>
#include <avr/interrupt.h>




int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_capture_initialize();

    sei();

    while(1)
    {
        while ( !isTimingReady() ) ;
        resetTimingReady();

        // some testing of time period between two consecutive
        // presses of a button connected to Timer1 capture input (PD.4)
        uint16_t inseconds = getTime_Period() / 1000000L;
        if (inseconds>63) inseconds=63;
        leds_set(inseconds);
        // this will display the time in seconds between subsequent releases of the button
    }

    return(0);
}


