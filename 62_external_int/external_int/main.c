//* testing TIMER1 interrupt - main.c *
#include "bios_external_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <avr/interrupt.h>

#define F_CPU (16000000UL)
#include <util/delay.h>


void MyFN45 (void)
{
    // This demo function shows how to associate an action with a particular interrupt
    leds_set(0); // reset LEDs, this will also reset the count in main loop
}

void MyFN67 (void)
{
    // This demo function shows how to associate action with several interrupts
    //      and check the source of the interrupt assuming pin_low activation.
    // In general it might be easier to have independent functions set for each pin.

    uint8_t inputs = keys_get();
    uint8_t status = leds_get();
    // uncomment only one of the two lines for the demo
    status = status ^ (inputs<<4);      // this will show that buttons bounce and trigger multiple times
    // status = status | (inputs<<4);   // this will show that the interrupt was triggered at least once
    leds_set( status ); // set LEDs based on input
}

void External_Int_Pins_Enable() {
    DDRD = DDRD  & ~0b00001100;
    PORTD = PORTD | 0b00001100;
}


int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    External_Int_Pins_Enable();
    External_Int0_initialize(EXT_INT_MODE_pin_falle, MyFN45);   // test each of the modes - this triggered once
    External_Int1_initialize(EXT_INT_MODE_pin_low,   MyFN45);   // test each of the modes - this triggered over and over again while button pressed
    sei(); // enable global interrupts, all circuitry must be initialized prior to this event

    uint8_t keep_busy = 0;
    while(1)
    {
        keep_busy++;
        if (keep_busy>=16)
            keep_busy = 0;

        cli();
        leds_set( (leds_get() & 0xF0 ) | keep_busy );
        sei();
        _delay_ms(500);

    }

    return(0);
}
