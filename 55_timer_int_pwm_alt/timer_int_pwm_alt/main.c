//* testing TIMER1 interrupt with software generated alt PWM - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "soft_pwm_ALT.h"
#include "bios_leds.h"
#include "bios_keys.h"

#define INT_FREQUENCY       50000
#define MAIN_LOOP_FREQUENCY 100

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

    Timer1_initialize( INT_FREQUENCY, &MyTimerFN, timer_prescale_1);
    //  make sure that FRQclock / prescaler < 65536 as Timer1 is a 16-bit timer

    // Set the PWM rates for the PWM channels
    // Note: the functions will compute the numbers to count based on percentages here
    uint8_t  pwm0 = 50;
    uint16_t pwmF = 128;
    PWM_generator_setFreq  (INT_FREQUENCY, pwmF);
    PWM_generator_setCoef (0, pwm0);
    PWM_generator_setCoef (1, 0);
    PWM_generator_setCoef (2, 0);
    PWM_generator_setCoef (3, 0);
    PWM_generator_setCoef (4, 0);
    PWM_generator_setCoef (5, 0);

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
            PWM_generator_setCoef (0, pwm0);
        }

        if ( (but_chg & B_K6) !=0 ) { // if ( (but_chg & 0b00000100) !=0 )
            if (pwmF<4096) {
                pwmF = pwmF * 2;
            } else {
                pwmF = 4096;
            }
            PWM_generator_setFreq  (INT_FREQUENCY, pwmF);
        }
        if ( (but_chg & B_K7) !=0 ) { // if ( (but_chg & 0b00001000) !=0 )
            if (pwmF>4) {
                pwmF = pwmF / 2;
            } else {
                pwmF = 4;
            }
            PWM_generator_setFreq  (INT_FREQUENCY, pwmF);
        }
    }

    return(0);
}
