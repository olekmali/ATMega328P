//* Simple test to test pin changed interrupt on PORTC *
#include <avr/io.h>
#include <avr/interrupt.h>

#include "bios_pinc_int.h"
#include "bios_leds.h"

void MyInterruptPinChangedFunction () {
    // Note: this is called from an interrupt -- no waiting inside
    // OK, just don't wait too long as the lab is in the remote mode
    // and the lab kit does not have capacitors for debouncing ":-("

    // this is an example that just does something visible
    // replace with your application code as needed
    leds_set( ~leds_get() );
    delay(1); // use of such long delay here is an abomination!
}


void setup_PC012() {
    DDRC =  DDRC  & 0b11111000; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins are input/zero
    PORTC = PORTC | 0b00000111; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins has pull up resistors/one

    // enable the interrupt on the lowest 3 pins of PORTC
    PCINT_PINC_initialize( 0b00000111, MyInterruptPinChangedFunction );   // enable pin change interrupt on PINC.0
}



int main(void)
{
    leds_init();
    setup_PC012();

    sei(); // enable all interrupts
    while (1)
    {
    }
}

