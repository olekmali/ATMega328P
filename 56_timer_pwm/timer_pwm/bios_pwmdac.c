//* Timer2 hardware PWM - implementation file *
#include "bios_pwmdac.h"
#include <avr/io.h>
#include <avr/interrupt.h>

void Timer2_PWM_initialize ()
{
    TCNT2 = 0;
    OCR2A  = 0;  // PWM at 0%
    TCCR2A = ( 1<<COM2A1 ) | (1<<WGM21 | 1<<WGM20);
            // Port pin Hi on bottom, Low on compare when fast PWM
    TCCR2B = (1 << CS20);
            // Prescale of 1 (fastest)
    // TIMSK = TIMSK & ~(1<<OCIE2A|1<<TOIE2);    // interrupts disabled (and were disabled to start with)
    // OC2A affects the Timer2 PWM pin alternative mode on PORTB3 - output mode must be enabled!
    DDRB = DDRB | 0b00001000;
}

void Timer2_PWM_shutdown ()
{
    DDRB   = DDRB & ~0b00001000;
    OCR2A  = 0;     // PWM at 0%
    TCCR2A = 0;     // shut down
    TCCR2B = 0;     // shut down
    TCNT2  = 0;
    TIMSK2 = 0;
    TIFR2  = 0;
}


void Timer2_PWM_setDuty (uint8_t duty)
{
    // OCR2 is hardware double-buffered in PWM generator mode
    OCR2A = duty;
}
