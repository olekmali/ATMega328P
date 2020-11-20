//* USART0 interrupt-based library - implementation file *
#include "bios_uart0_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define xtal 16000000L

static void (*UART0_RX_ISR_function)(char) = 0;

void uart0_initialize( uint16_t baud, void (*handle_rx)(char) )
{
    uint32_t temp = xtal/16/baud-1;
    UBRR0H = (temp >> 8) & 0x0F;
    UBRR0L = (temp & 0xFF);

    UART0_RX_ISR_function = handle_rx;

    // Set frame format: 8data, 2stop bit
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);

    UCSR0B = (1<<RXEN0)|(1<<TXEN0) |(1<<RXCIE0);
                            //   ^^^^^^^^^^^ data received interrupt enabled (when global interrupts are enabled)
}

void uart0_shutdown ()
{
    UCSR0B = 0;
}


uint8_t uart0_ready_TX ()
{
    return ( 0 != (UCSR0A & 1<<UDRE0) );
}

void uart0_putc (char c)
{
    while( 0 == (UCSR0A & 1<<UDRE0) ) ;
    UDR0 = c;
}

void uart0_puts (const char* const s)
{
    for ( const char* p = s; *p!='\0'; ++p )
    uart0_putc(*p);
}

ISR(USART_RX_vect)
{
    char value = UDR0;                   // read UART register into value
    UART0_RX_ISR_function(value);      // call the _SHORT_ user defined function to handle it
}
