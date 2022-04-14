//* Pin change interrupt-based library - implementation file *
#include "bios_pinc_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*PCINT_PINC_function)(void) = 0;

void PCINT_PINC_initialize ( uint8_t which_pins, void (*handle_fn)(void) )
{
    PCINT_PINC_function = handle_fn;
    PCIFR  = PCIFR  & ~(1<<PCIF1);  // clear pending interrupts if any
    PCICR  = PCICR  | 1<<PCIE1;     // enable group PINC PINC interrupts
    PCMSK1 = which_pins;            // enable requested pins of PORTC
}


void PCINT_PINC_shutdown ()
{
    PCMSK1 = 0;                     // disable all individual PINC changed interrupts
    PCICR  = PCICR  & ~(1<<PCIE1);  // disable group PINC PINC interrupts
    PCIFR  = PCIFR  & ~(1<<PCIF1);  // clear pending interrupts if any
}


ISR(PCINT1_vect)
{
    PCINT_PINC_function();
}
