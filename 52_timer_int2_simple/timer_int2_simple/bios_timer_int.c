//* Timer1 interrupt-based library - implementation file *
#include "bios_timer_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define xtal 16000000L

static void (*Timer1_Overflow_function)(void) = 0;

void Timer1_initialize ( uint32_t frequency, void (*handle_overflow)(void), uint8_t prescaler )
{
                                      // -  1  8 64 256 1024
    const static uint8_t prescales[] = {0, 0, 3, 6, 8, 10};

    Timer1_Overflow_function = handle_overflow;

    TCNT1=0x00;
    // set timer0 counter initial value to 0
    // (note: entirely redundant as it was never used before)

    OCR1A = ((uint32_t)(xtal)>>prescales[prescaler]) / frequency - 1;

    TCCR1A = 0x00;
    // AABBCCMM - lower two bits of 4-bit mode MMMM

    TCCR1B = 0x08 | (0x07 & prescaler);
    // ??-MMPPP - MM is upper two bits of mode MMMM, PPP is clock rescale
    // where PPP of 011 is clock rescale of 64
    // where MM MM is the Timer mode, we use 01 00 - CTC

    TIFR1  = 0;
    // reset any pending Timer1 interrupts
    // (note: entirely redundant as none is pending - it was never used before)

    TIMSK1 = (1<<OCIE1A);
        //  ICIE1  - enable Timer1 capture interrupt
        //  OCIE1A - enable Timer1 match OCR1A match interrupt
        //  TOIE1  - enable Timer1 overflow interrupt
}

void Timer1_shutdown ()
{
    TIMSK1 &= ~(1<<TOIE1 | 1<<OCIE1A | 1<<OCIE1B | 1<<TOIE1);
    // disable all Timer1 interrupts
    // which is the same effort as disabling just one interrupt but can be reused in other examples

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
    TIMSK1 = 0;
    TIFR1  = 0;
}

ISR(TIMER1_COMPA_vect)
{
    // Note:    if time is critical (high frequency interrupt that
    //          is optimized for speed then put the code here
    //          instead of calling it in another function
    Timer1_Overflow_function();
}
