//* External interrupt-based library - implementation file *
#include "bios_external_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static void (*External_interrupt0_function)(void) = 0;
static void (*External_interrupt1_function)(void) = 0;

void External_Int0_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt0_function = handle_fn;
    External_Int0_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT0;
    EIFR  = EIFR  | 1<<INTF0;
}

void External_Int1_initialize ( uint8_t mode, void (*handle_fn)(void) )
{
    External_interrupt1_function = handle_fn;
    External_Int1_chg_mode(mode);
    EIMSK = EIMSK | 1<<INT1;
    EIFR  = EIFR  | 1<<INTF1;
}

void External_Int0_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT0);
}

void External_Int1_shutdown ()
{
    EIMSK = EIMSK & ~(1<<INT1);
}

void External_Int0_chg_mode (uint8_t mode)
{
    uint8_t sreg_save = SREG;                           // Preserve Current Interrupt Status
    cli();
    uint8_t eicra_old = EICRA & ~(1<<ISC01|1<<ISC00);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRA = eicra_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRA = eicra_old | 1<<ISC00;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRA = eicra_old | 1<<ISC01;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRA = eicra_old | 1<<ISC01 | 1<<ISC00;
        break;
    } // switch
    SREG = sreg_save;                                   // restore interrupts
}

void External_Int1_chg_mode (uint8_t mode)
{
    cli();
    uint8_t eicra_old = EICRA & ~(1<<ISC11|1<<ISC10);   // remove the old mode status
    switch (mode)
    {
        case EXT_INT_MODE_pin_low:
            EICRA = eicra_old;
        break;
        case EXT_INT_MODE_pin_hi:
            EICRA = eicra_old | 1<<ISC10;
        break;
        case EXT_INT_MODE_pin_falle:
            EICRA = eicra_old | 1<<ISC11;
        break;
        case EXT_INT_MODE_pin_raise:
            EICRA = eicra_old | 1<<ISC11 | 1<<ISC10;
        break;
    } // switch
    sei();
}


ISR(INT0_vect)
{
    External_interrupt0_function();
}

ISR(INT1_vect)
{
    External_interrupt1_function();
}
