//* Simple test to sleep and wake up on Timer1 output compare interrupt *
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

#include "bios_leds.h"
#include "bios_timer_int.h"

//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define INT_FREQUENCY         1L
#define MAIN_LOOP_WAKEUP     10 // do somehting only once every 10 interrupts

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t semaphore = 0;  // volatile keyword is very important here!


//------------------------------------------------------------------------------------
// HAL Functions
//------------------------------------------------------------------------------------
inline void sleep_On()
{
    set_sleep_mode(SLEEP_MODE_IDLE); // Idle is not as good as SLEEP_MODE_POWER_*
    sleep_enable();
    sleep_cpu();
}

inline void sleep_Off()
{
    sleep_disable();
}

//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    leds_set( leds_get() | B_L5 ); // for oscilloscope debugging only
    // Remember: we are inside the interrupt routine

    semaphore = 1;
    sleep_Off();

    leds_set( leds_get() & ~B_L5 ); // for oscilloscope debugging only
}




void do_something() {
    leds_set( leds_get() | B_L4 );
    _delay_ms(500);
    // wdt_reset(); use WDT restet intermitently
    // if the whole task takes more than 1.7 seconds
    leds_set( leds_get() & ~B_L4 );
}


int main(void)
{
    wdt_enable(7); //// 7 - 1.8s
    leds_init();
    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1024 );
    // Note: 1024 prescaler allows for the interrupt to go as low as 0.25Hz @ 16MHz clock

    uint8_t loop_count = 0;
    sei(); // enable all interrupts
    while (1)
    {
        if ( semaphore!=0 ) {
            // we missed the deadline already!
            leds_set( leds_get() | B_L3 ); // indicate an error
        }

        while ( semaphore==0 ) {
            wdt_reset();
            sleep_On();
        }
        semaphore = 0;

        if ( 0 == loop_count) {
            loop_count = MAIN_LOOP_WAKEUP;
            do_something(); // this represents some activity before the board goes to sleep again
        } else {
            loop_count--;
        }
    }
}
