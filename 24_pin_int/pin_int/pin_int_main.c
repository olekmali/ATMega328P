//* Simple test to test pin changed interrupt on PORTC *
#include <avr/io.h>
#include <avr/interrupt.h>

#include <stdint.h>
#include "bios_leds.h"
#include "bios_pinc_int.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

void setup_PC012() {
    DDRC =  DDRC  & 0b11111000;
    PORTC = PORTC | 0b00000111;
}
uint8_t getPC012() {
    return( PINC & 0b00000111 );
}


void MyInterruptHander() {
    // Note: this is called from an interrupt -- no waiting inside
    // OK, just don't wait too long as the lab is in the remote mode
    // and the lab kit does not have capacitors for debouncing ":-("

    // this is an example that just does something visible
    // replace with your application code as needed
    uint8_t tmp = leds_get();
    uint8_t src = PINC;
    if ( (src & 0b00000101)==0 ) { /* do something on pressing button connected to Pin C.1 */}
    tmp =tmp ^ (1<<3); // do something to show that the interrupt works works
    leds_set(tmp);
    _delay_ms(1); // use of almost any delay, and especially such long delay here is an abomination!

}


int main()
{
    leds_init();
    setup_PC012();
    PCINT_PINC_initialize( 0b00000111, MyInterruptHander );

    sei();
    while(1)
    {
    }
    return(0);
}
