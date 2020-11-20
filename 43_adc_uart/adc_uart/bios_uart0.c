//* USART0 library - implementation file - 2019 *
#include "bios_uart0.h"
#include <avr/io.h>

#define xtal 16000000L

void uart0_initialize(uint16_t baud)
{
    // set up baud rate
    uint32_t temp = (xtal/16)/baud-1; // Note: UBRR can hold value 0..4095 (not up to 65535)
    UBRR0H = (temp >> 8) & 0x0F;
    UBRR0L = (temp & 0xFF);

    // enable both sending and receiving
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // Set frame format: 8 data bits, 2 stop bits, no parity
    UCSR0C = (3<<UCSZ00)|(1<<USBS0);
}

void uart0_initialize3 (uint16_t baud, uint8_t uart_stop_mode, uint8_t uart_parity_mode)
{
    // set up baud rate
    uint32_t temp = xtal/16/baud-1;
    UBRR0H = (temp >> 8) & 0x0F;
    UBRR0L = (temp & 0xFF);

    // enable both sending and receiving
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);

    // Set frame format: 8 data bits, uart_stop_mode stop bit, uart_parity_mode parity type
    UCSR0C = (3<<UCSZ00)|(1<<USBS0);  // To be replaced by the line below
    // UCSR0C =  (3<<UCSZ0) | ( (uart_stop_mode & 0x01) << ___ ) | ( (uart_parity_mode & 0x03) << ___ ); // to be added
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

uint8_t uart0_ready_RX ()
{
    return ( 0 != (UCSR0A & 1<<RXC0) );
}

char uart0_getc ()
{
    while( 0 == (UCSR0A & 1<<RXC0) ) ;
    return ( (uint8_t) UDR0 );
}

char uart0_getc_echo ()
{
   char c = uart0_getc();
   uart0_putc(c); // this is console echo for a live user
   return c;
}

uint8_t uart0_check_error()
{
    // checks for Frame Error, Data OverRun, and Parity Error - 0b000eee0
    return( UCSR0A & ((1<<FE0)|(1<<DOR0)|(1<<UPE0)));
}

size_t uart0_gets_echo (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size; // to allow space for '\0'

    while (1) {
        c=uart0_getc();
        if (c=='\b'||c==127) {
            uart0_putc('\a');
        } else if (c=='\n'||c=='\r') {
            uart0_putc('\r');
            uart0_putc('\n');
            *s=0;
            break;
        } else {
            if (count<size) {
                *s++=c; // *s=c; ++s;
                ++count;
                uart0_putc(c);
            } else {
                uart0_putc('\a');
            }
        }
    }
    return(count);
}

size_t uart0_gets (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size;

    while (1) {
        c=uart0_getc_echo();
        if ( c=='\n' || c=='\r' || count>=size )
        break;
        // return if new line, return or buffer full
        *s++=c; // *s=c; ++s;
        ++count;
    }
    *s=0; // remember to terminate the char array string
    return(count);
}

size_t uart0_gets_edit (char* s, size_t size)
{
    char c;
    size_t count=0;
    --size; // to allow space for '\0'

    while (1) {
        c=uart0_getc();
        switch(c) {
            case '\b': // erase (^H)
            case 127: // backspace (^?)
                if (count) {
                    uart0_putc('\b');
                    uart0_putc(' ');
                    uart0_putc('\b');
                    --s;
                    --count;
                }
                break;
            case '\n':
            case '\r': // CR or LF
                uart0_putc('\r');
                uart0_putc('\n');
                *s=0;
                return count;
                default:
                if (count<size) {
                    *s++=c; // *s=c; ++s;
                    ++count;
                    uart0_putc(c);
                } else {
                    uart0_putc('\a');
                }
                break;
        }
    }
    return(count);
}

size_t uart0_write (const void* const s, size_t size)
{
    const uint8_t * p =s;
    size_t i=0;
    while ( i<size )
    {
        uart0_putc(*p);
        ++p;
        ++i;
    }
    return(i);
}

size_t uart0_read (void* s, size_t size)
{
    uint8_t * p =s;
    char c;
    size_t count=0;

    while (count<size) {
        c=uart0_getc();
        *p++=c;
        ++count;
    }
    return(count);
}
