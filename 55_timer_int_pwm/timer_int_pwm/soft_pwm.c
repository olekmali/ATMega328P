//* soft_pwm.c *
#include "soft_pwm.h"

#include <avr/interrupt.h>
#include "bios_leds.h"

//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static pwm_cnt_t MyPWMCoef_params[PWM_CH_MAX] = {0};
static pwm_cnt_t MyPWMCoef_reload[PWM_CH_MAX] = {0};
static uint8_t   MyPWMCoef_change = 0;

//------------------------------------------------------------------------------------
// Functions used to access the global variables shared with interrupts
//------------------------------------------------------------------------------------
void PWM_generator_setParam(uint8_t channel, pwm_cnt_t percentage)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();

    if ( PWM_100_PERCENT < percentage)
        percentage = PWM_100_PERCENT;

    if ( PWM_CH_MAX>channel )
    {
        MyPWMCoef_reload[channel] = ( percentage * ((uint16_t)PWM_RESOLUTION) ) / PWM_100_PERCENT;
        MyPWMCoef_change = 1;
        // Note: Actual change in PWM will take effect when a new PWM cycle starts to prevent glitches
    }

    SREG = sreg_save;                   // restore interrupts
}


//------------------------------------------------------------------------------------
// Internal Functions
//------------------------------------------------------------------------------------
void MyPWM_generator_copyParam(void)
{
    for (uint8_t i=0; i<PWM_CH_MAX; ++i)
        MyPWMCoef_params[i] = MyPWMCoef_reload[i];
    MyPWMCoef_change = 0;
}


//------------------------------------------------------------------------------------
// Timer Interrupt-Run Functionality
//------------------------------------------------------------------------------------
void PWM_generator_interrupt (void)
{
    // static variable - a global variable hidden in a function
    static pwm_cnt_t counter = 0;

    // PWM software counter update
    if ( PWM_RESOLUTION <= counter )
    {
        counter = 0;
        if ( MyPWMCoef_change )
        {
            MyPWM_generator_copyParam();
            // Note: Actual change in PWM will take effect when a new PWM cycle starts
        }
    }
    else
    {
        counter++;
    }

    // PWM output computation based on software counter
    uint8_t pwmbyte = 0;

    uint8_t current = 0x01;
    for (uint8_t i=0; i<PWM_CH_MAX; ++i)
    {
        if ( counter < MyPWMCoef_params[i] ) pwmbyte |= current;
        // else do not set that bit as ON

        current <<= 1;
    }

    leds_set( pwmbyte | (leds_get() & ~PWM_CH_MASK) );
}
