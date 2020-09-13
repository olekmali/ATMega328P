//* testing TIMER1 interrupt with software generated PWM - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "soft_pwm.h"
#include "bios_leds.h"
#include "bios_keys.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define PWM_FREQUENCY       1000L
#define INT_FREQUENCY       (PWM_FREQUENCY * PWM_RESOLUTION)
#define MAIN_LOOP_FREQUENCY 100


//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t     semaphore = 0;  // volatile keyword is very important here!


//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // Remember: we are inside the interrupt routine
    //              - no excessive loops
    //              - no time consuming tasks
    //              - no interrupt enabling
    PWM_generator_interrupt();

    static uint16_t counter = 0;
    //     ^^^^^^^^ make sure that ( SAMPLING__FRQ / MAIN_LOOP_FREQUENCY )
    // fits the variable range! uint8_t - 255, uint16_t - 65535
    if (0<counter) counter--;
    else
    {
        counter = ( INT_FREQUENCY / MAIN_LOOP_FREQUENCY );

        semaphore = 1;
    }
}


int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( INT_FREQUENCY , MyTimerFN, timer_prescale_1 );

    // Set the PWM rates for the PWM channels
    // Note: the functions will compute the numbers to count based on percentages here
    uint8_t pwm0 = 0, pwm1 = 100;
    PWM_generator_setParam (0, pwm0);
    PWM_generator_setParam (1, pwm1);

    sei();

    uint8_t but_prev = 0;
    while(1) {
        // wait for the signal to proceed from the interrupt
        while ( semaphore==0 )
            ;
        // and immediately reset that signal to wait state
        semaphore = 0;

        uint8_t but_cur = keys_get();
        uint8_t but_chg = ( but_cur^but_prev ) & but_cur;
        but_prev = but_cur; // important! update what is now current will be past next time

        // cycle through five PWM levels with one button
        if ( (but_chg & B_K4) !=0 ) { // if ( (but_chg & 0b00000001) !=0 )
            if (pwm0<100) {
                pwm0 = pwm0 + 20;
            } else {
                pwm0 = 0;
            }
            PWM_generator_setParam (0, pwm0);
        }

        if ( (but_chg & B_K6) !=0 ) { // if ( (but_chg & 0b00000100) !=0 )
            if (pwm1<80) {
                pwm1 = pwm1 + 20;
            } else {
                pwm1 = 100;
            }
            PWM_generator_setParam (1, pwm1);
        }
        if ( (but_chg & B_K7) !=0 ) { // if ( (but_chg & 0b00001000) !=0 )
            if (pwm1>20) {
                pwm1 = pwm1 - 20;
            } else {
                pwm1 = 0;
            }
            PWM_generator_setParam (1, pwm1);
        }


    }

    return(0);
}
