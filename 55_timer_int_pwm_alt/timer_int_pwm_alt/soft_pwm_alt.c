//* soft_pwm.c *
#include "soft_pwm_alt.h"

#include <avr/interrupt.h>
#include "bios_leds.h"

//------------------------------------------------------------------------------------
// Global constant(s)
//------------------------------------------------------------------------------------
#define PHASE_PREC      (65536L)            // range of phase accumulator
#define PHASE_HALF      (PHASE_PREC>>1)     // half of range of phase accumulator

//------------------------------------------------------------------------------------
// Global variable(s) used as bridge to pass parameters to the interrupts
//------------------------------------------------------------------------------------
static volatile uint16_t MyPWMCoef_params[PWM_CH_MAX] = {0};
static volatile uint16_t MyPWMCoef_reload[PWM_CH_MAX] = {0};
static volatile uint16_t MyPWMFreq_params = 0;
static volatile uint16_t MyPWMFreq_reload = 0;
static volatile uint8_t  MyPWMCoef_change = 0;

//------------------------------------------------------------------------------------
// Functions used to access the global variables shared with interrupts
//------------------------------------------------------------------------------------
void PWM_generator_setCoef(uint8_t channel, uint8_t percentage)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();

    if ( PWM_100_PERCENT < percentage)
        percentage = PWM_100_PERCENT;

    if ( PWM_CH_MAX>channel )
    {
        MyPWMCoef_reload[channel] = ( (uint32_t)percentage * (uint32_t)PHASE_PREC ) / PWM_100_PERCENT;
        MyPWMCoef_change = 1;
        // Note: Actual change in PWM will take effect when a new PWM cycle starts to prevent glitches
    }

    SREG = sreg_save;                   // restore interrupts
}


void PWM_generator_setFreq(uint32_t int_frq, uint16_t pwm_frq)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();

    MyPWMFreq_reload = ( (uint32_t)pwm_frq * (uint32_t)PHASE_PREC ) / int_frq;
    MyPWMCoef_change = 1;
    // Note: Actual change in PWM will take effect when a new PWM cycle starts to prevent glitches

    SREG = sreg_save;                   // restore interrupts
}


//------------------------------------------------------------------------------------
// Internal Functions
//------------------------------------------------------------------------------------
void MyPWM_generator_copyParam(void)
{
    for (uint8_t i=0; i<PWM_CH_MAX; ++i)
    {
        MyPWMCoef_params[i] = MyPWMCoef_reload[i];
        MyPWMFreq_params    = MyPWMFreq_reload;
    }
    MyPWMCoef_change = 0;
}


//------------------------------------------------------------------------------------
// Timer Interrupt-Run Functionality
//------------------------------------------------------------------------------------
void PWM_generator_interrupt (void)
{
    // static variable - a global variable hidden in a function
    static uint16_t phase_current  = 0;     // holds phase accumulator, Note: will roll over at 65536
    static uint16_t phase_last;             // used to detect phase roll over through 0

    phase_last     = phase_current;
    phase_current += MyPWMFreq_params;      // increment phase accumulator

    if ( MyPWMCoef_change /* && (phase_current<phase_last)*/ )
    {
        // Note: rollover detected when (phase_current<phase_last)
        MyPWM_generator_copyParam();
    }

    // PWM output computation based on software counter
    uint8_t pwmbyte = leds_get() & ~PWM_CH_MASK;

    uint8_t curLED = 0b00000001;
    for (uint8_t i=0; i<PWM_CH_MAX; ++i)
    {
        if ( phase_current < MyPWMCoef_params[i] ) {
            pwmbyte |= curLED;
        } // else do not set that bit as ON

        curLED <<= 1; // curLED = curLED << 1;
    }

    leds_set(pwmbyte);
}
