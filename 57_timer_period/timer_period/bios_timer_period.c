//* Timer1 interrupt-based library - implementation file *
#include "bios_timer_period.h"
#include <avr/io.h>
#include <avr/interrupt.h>


void Timer1_capture_initialize ()
{
    // PortD pin 4 is input with pull up
    DDRD  = DDRD  & ~(1<<4);
    PORTD = PORTD |   1<<4;

    // Predivide Clock by 8 will yield one counter tick to be 0.5 uS
    // together with additional software timer overflow count that allows
    // for accurate measuring both long time periods as well as short ones

    TCNT1=0x00;                     // set timer0 counter initial value to 0

    TCCR1A = 0x00;          // AABBCC00
    TCCR1B = 1<<ICNC1       | 1<<ICES1      | 0x02;
    //       filter input     enable capture edge
    //                                     11-00PPP where PPP == 010is clock prescale of 8

    TIMSK1 = 1<<ICIE1 | 1<<TOIE1;  // |= to preserve other Timer interrupt settings
        //  ICIE1  - enable Timer1 capture interrupt
        //  OCIE1A - enable Timer1 match OCR1A match interrupt
        //  TOIE1  - enable Timer1 overflow interrupt
}

void Timer1_shutdown ()
{
    TIMSK1 &= ~(1<<TOIE1 | 1<<OCIE1A | 1<<OCIE1B | 1<<ICIE1 | 1<<TOIE1);
    // disable all Timer1 interrupts
    // which is the same effort as disabling just one interrupt but can be reused in other examples

    TCCR1A = 0;
    TCCR1B = 0;
    TCCR1C = 0;
}

static volatile uint16_t ov_counter    = 0; // to measure time longer than 64K uS
static volatile uint16_t raising_edge, falling_edge;
static volatile uint32_t clocks_high   = 0;
static volatile uint32_t clocks_period = 0;
static volatile uint8_t  new_results   = 0;

ISR(TIMER1_OVF_vect)
{
    ov_counter++;
    // if desired this can be used for time periods longer than 65.5ms
}

ISR(TIMER1_CAPT_vect)
{
    // if ICP1 is pulled up
    if( TCCR1B & (1<<ICES1) ){
        uint16_t new_raising_edge = ICR1;
//      clocks_period   = new_raising_edge - raising_edge + ov_counter * 0x10000L;
        clocks_period   = new_raising_edge - raising_edge +  ( ((uint32_t)ov_counter)<<16 );
        raising_edge    = new_raising_edge;
        //preparing to detect a falling edge
        TCCR1B = TCCR1B & ( ~(1<<ICES1) );
        ov_counter      = 0;
        new_results     = 1; // optional to detect a new set of measurements
    } else {
        falling_edge = ICR1;
        // preparing for next raising edge
        TCCR1B = TCCR1B | 1<<ICES1;
        clocks_high = falling_edge - raising_edge + ov_counter * 0x10000;
    }
}


uint32_t getTime_Period (void)
{
    cli();
    uint32_t value = clocks_period>>1;
    sei();
    return(value);
}

uint32_t getTime_High (void)
{
    cli();
    uint32_t value = clocks_high>>1;
    sei();
    return(value);
}

uint8_t isTimingReady ( void )
{
    return(new_results);
}

void resetTimingReady ( void )
{
    new_results = 0;
}
