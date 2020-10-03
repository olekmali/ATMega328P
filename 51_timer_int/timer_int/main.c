//* Introductory examples for TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU (16000000UL)
#include <util/delay.h>


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define INT_FRQ     (10)
#define KEY_MODE    (3)
#define LED_MAIN    (2)
#define LED_OUT     (0)


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    static uint8_t extended_counter = 0;
    extended_counter++;

    if ( 9==extended_counter )
    {
        leds_set( leds_get() | (1<<LED_OUT) );
    }

    if ( 10==extended_counter )
    {
        leds_set( leds_get() & ~(1<<LED_OUT) );
        extended_counter = 0;
    }
}


int main1(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    while (1)
    {
        cli();
        leds_set( leds_get() ^ (1<<LED_MAIN) );
        sei();
    }

    return(0);
}


int main2(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    while (1)
    {
        // cli(); // Exam qestion: what eventually goes wrong in this case?
        leds_set( leds_get() ^ (1<<LED_MAIN) );
        // sei();
    }

    return(0);
}

int main3(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    uint8_t state = 0;
    while (1)
    {
        uint8_t new_state = keys_get() & (1<<KEY_MODE);
        if ( state != new_state && state /* !=0 */ )
        {
            cli();
            leds_set( leds_get() ^ (1<<LED_MAIN) );
            sei();
        }
        state = new_state;
        _delay_ms(20);
    }

    return(0);
}

int main4(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FRQ, MyTimerFN, timer_prescale_64 );
    // Note: for xtal of 16MHz, 16-bit timer and prescale of 64
    // we got interrupt frequency range of ____ Hz min, _____ kHz max (Exam question)

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event
    // regular toggle LED test follows

    uint8_t state = 0;
    while (1)
    {
        uint8_t new_state = keys_get() & (1<<KEY_MODE);
        if ( state != new_state && state /* !=0 */ )
        {
            // cli(); // Exam qestion: what eventually goes wrong in this case
                      //               and why it is more rare than in case of main2()?
            leds_set( leds_get() ^ (1<<LED_MAIN) );
            // sei();
        }
        state = new_state;
        _delay_ms(20);
    }

    return(0);
}



int main(void)
{
    return( main3() ); // Test using main 1, 2, 3, and 4. Consider exercises described in comments
}
		