//* testing TIMER1 interrupt with software generated PWM for servomotor - main.c *
#include "bios_timer_int.h"
#include <stdint.h>
#include <avr/interrupt.h>

#include "bios_leds.h"
#include "bios_keys.h"


//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
typedef uint8_t pwmcnt_t;

#define PWM_FREQUENCY        50L
#define SERVO_MIN_US        500L
#define SERVO_STEPS          10

#define PWM_RESOLUTION      200     // 0.5%
#define INT_FREQUENCY   ( PWM_FREQUENCY * PWM_RESOLUTION )
#define PWM_SERVO_START       5     // 2.5%
#define PWM_SERVO_STEP        2     // 1.0%

#define MAIN_LOOP_FREQUENCY 100

pwmcnt_t position_servo(uint8_t position)
{
    return( PWM_SERVO_START + position * PWM_SERVO_STEP );
}



//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint8_t     semaphore = 0;  // volatile keyword is very important here!

static volatile pwmcnt_t    set_pwm0 = 0;  // volatile keyword is important here!
static volatile pwmcnt_t    set_pwm1 = 0;  // volatile keyword is important here!
// Note: if a shared variable is larger than 8bit then its update is not so called atomic
//      and interrupts have to be suspended while a variable is modified in main function

//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    // Remember: we are inside the interrupt routine
    //              - no excessive loops
    //              - no time consuming tasks
    //              - no interrupt enabling
    { // this block for debugging / verification only
        static uint16_t verify = 0;         // data type needs to hold value of INT_FREQUENCY / 2
        if ( 0 == verify ) {
            verify = (uint16_t)(INT_FREQUENCY / 2);   // This should make LED.5 blink with a period of 1 sec.
            leds_set( leds_get() ^ B_L5 );
        } else {
            verify--;
        }
    }


    static pwmcnt_t pwm_rate0 = 0;   // PWM rate only for the current time period
    static pwmcnt_t pwm_rate1 = 0;   // PWM rate only for the current time period
    static pwmcnt_t pwm_counter = 0; // note: static == hidden global variable
    //     ^^^^^^^^ make sure that ( SAMPLING__FRQ / MAIN_LOOP_FREQUENCY )
    // fits the variable range! uint8_t - 255, uint16_t - 65535
    if (0<pwm_counter) {
        pwm_counter--;
    } else {
        pwm_counter = PWM_RESOLUTION;
        // we do not want PWM rate to change in the middle of a PWM cycle
        pwm_rate0 = set_pwm0;
        pwm_rate1 = set_pwm1;
    }

    // PWM action section
    uint8_t curLEDs = leds_get();
    if ( pwm_counter<pwm_rate0 ) {
        curLEDs = curLEDs | B_L0;
    } else {
        curLEDs = curLEDs & ~B_L0;
    }
    if ( pwm_counter<pwm_rate1 ) {
        curLEDs = curLEDs | B_L1;
    } else {
        curLEDs = curLEDs & ~B_L1;
    }
    leds_set(curLEDs);

    // synchronizing the main loop
    static pwmcnt_t semaphore_counter = 0; // note: static == hidden global variable
    //     ^^^^^^^^ make sure that ( SAMPLING__FRQ / MAIN_LOOP_FREQUENCY )
    // fits the variable range! uint8_t - 255, uint16_t - 65535
    if (0<semaphore_counter) {
        semaphore_counter--;
    } else {
        semaphore_counter = ( INT_FREQUENCY / MAIN_LOOP_FREQUENCY );
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
    // Note: PWMs needs to be in range 0...PWM_RESOLUTION, not in %
    uint8_t sw_level0 = 0;                  // = 0%
    uint8_t sw_level1 = SERVO_STEPS;        // = 100%
    set_pwm0 = position_servo(sw_level0);
    set_pwm1 = position_servo(sw_level1);

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
            if (sw_level0<SERVO_STEPS) {
                sw_level0++;
            } else {
                sw_level0 = 0;
            }
            cli();
            set_pwm0 = position_servo(sw_level0);
            sei();

        } // else nothing. Technically several buttons may be depressed during the same time interval

        if ( (but_chg & B_K6) !=0 ) { // if ( (but_chg & 0b00000100) !=0 )
            if (sw_level1<SERVO_STEPS) {
                sw_level1++;
                cli();
                set_pwm1 = position_servo(sw_level1);
                sei();
            } // else nothing;
        } // else nothing. Technically several buttons may be depressed during the same time interval

        if ( (but_chg & B_K7) !=0 ) { // if ( (but_chg & 0b00001000) !=0 )
            if (sw_level1>0) {
                sw_level1--;
                cli();
                set_pwm1 = position_servo(sw_level1);
                sei();
            } // else nothing;
        } // else nothing. Technically several buttons may be depressed during the same time interval

    }

    return(0);
}
