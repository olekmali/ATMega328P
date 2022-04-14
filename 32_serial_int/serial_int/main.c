//* testing USART0 with receive data interrupt - main.c *
#include "bios_uart0_int.h"
#include "bios_leds.h"
#include <stdlib.h>
#include <avr/interrupt.h>

#define F_CPU (16000000UL)
#include <util/delay.h>


void MyCharReceivedFN (char c)
{
    // This function is called from within an interrupt
    // It must execute quickly - no loops or delays

    // indicate that an interrupt is called by displaying the 7-bit char number
    // the 8th bit is preserved for toggling in main
    leds_set( ( leds_get()&0b00100000 )  |  ( c & 0b00011111 ) );

    if ( uart0_ready_TX() ) uart0_putc(c); // echo only if that would not stall
}


int main(void)
{
    uart0_initialize(uart_bps_9600, MyCharReceivedFN);

    leds_init();
    leds_set(0);

    sei(); // enable global interrupts, all circuitry must be initialized prior to this event

    while (1)
    {
        cli();
        // This reads and writes the same resource that is used inside the interrupt
        // The interrupt must not suspend this sequence of operations and modify the resource mean while
        // or data integrity will be lost in such rare but possible case
        leds_set( leds_get()^0b00100000 );  // indicate that the main loop is "still alive"
        sei();

        _delay_ms(500);
    }
    return(0);
}


