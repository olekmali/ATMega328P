//* testing USART0 - main.c *
#include "bios_uart0.h"
#include "bios_leds.h"
#include <stdlib.h>
#include <avr/io.h>
#include <string.h> // used for strlen in some tests

#define F_CPU (16000000UL)
#include <util/delay.h>

int main_test1_putc_connection(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        uart0_putc('A');
        _delay_ms(1000);    // remove the delay to check what is the transmission speed
                        // limited only bu the UART transmission speed
    }
    return(0);
}



int main_test2_puts(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        uart0_puts("sample text\r\n"); // experiment with the new line character(s)
                        // try \r\n - Windows, \r - MacOS, \n Unix. Linux, and nothing
                        // if interested, see also: https://en.wikipedia.org/wiki/Newline
                        // also try to put \r\n in the middle of the text
                        // also try to put \0 in the middle of the text (-- will cut off the remainder)
        _delay_ms(5000);
    }
    return(0);
}



int main_test3_write(void)
{
    const char str[]  = "sample \0text\r\n";
    const uint8_t len = sizeof(str); // you can use strlen if there is no \0 inside

    leds_init();
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        uart0_write(str, len); // binary write will write anything, including \0 in the middle
        _delay_ms(5000);
    }
    return(0);
}



int main_test4_readyRX(void)
{
    leds_init();
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        leds_set(0);
        if ( uart0_ready_RX() )
        {
            leds_set( uart0_getc() );   // display character ASCII code as number
            _delay_ms(1000);                // and keep on the display for one second

            // note that only two characters can be kept in a receiving buffer
            // while the program does not read from the UART. All else is lost
        }
    }
    return(0);
}



int main_test5_readyTX(void)
{
    leds_init();
    uart0_initialize(uart_bps_9600);

    char c='A';
    while (1)
    {
        if ( uart0_ready_TX() )
        {
            leds_set(0xFF);
            uart0_putc(c);
            if (c<'Z') ++c; else c='A';
            leds_set(0x00);
        }
    }
    return(0);
}



int main_test6_getc_echo(void)
{
    leds_init();
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        leds_set( uart0_getc_echo() );   // display character ASCII code as number
        // this time we are testing reading with an echo for a human being on the other side
    }
    return(0);
}



int main_test7_getc_puts(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[10]; // make sure that it is large enough - worst case number of digits plus one

        uint8_t b = uart0_getc(); // technically a byte is just a byte
        // but please do use uint8_t or int8_t when you consider the byte as a number
        // char may be signed or unsigned by default depending on a compiler

        uart0_puts("code: 0x");
        uart0_puts( itoa( b, buffer, 16 ) );
        uart0_puts(" which is 0b");
        uart0_puts( itoa( b, buffer, 2 ) );
        uart0_puts(" which is ");
        uart0_puts( itoa( b, buffer, 10 ) );
        uart0_putc('\r');
        uart0_putc('\n');
    }
    return(0);
}



int main_test8_gets(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[16];
       uart0_puts("Please enter some text up to 15 characters:");
        uart0_gets_echo(buffer, sizeof(buffer));
    }
    return(0);
}



int main_test9_gets_edit(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[16];
        uart0_puts("Please enter some text up to 15 characters (with editing):");
        uart0_gets_edit(buffer, sizeof(buffer));
    }
    return(0);
}



int main_test10_atoi(void)
{
    uart0_initialize(uart_bps_9600);

    while (1)
    {
        char buffer[6];
        uart0_puts("Please enter an integer number:");
        uart0_gets(buffer, sizeof(buffer));
        uart0_puts("\r\n"); // check what happens to the printout if you remove this line

        uint16_t b = atoi(buffer);
        if (b==0) {
            uart0_puts("You entered 0 or not a number\r\n");
        } else if (b<256) {
            uart0_puts("The ASCII character for this number is '");
            uart0_putc( (uint8_t)b );// we say to the compiler: it is OK to truncate to 8 bits.
            uart0_puts("'\r\n");
        } else {
            uart0_puts("You entered a number > 255\r\n");
        }

    }
    return(0);
}



int main_testB_lower_speed(void)
{
    uart0_initialize(uart_bps_2400);

    while (1)
    {
        uart0_puts("sample text\r\n");
        _delay_ms(5000);
    }
    return(0);
}



int main_testC_higher_speed(void)
{
    uart0_initialize(uart_bps_57600);

    while (1)
    {
        uart0_puts("sample text\r\n");
        _delay_ms(5000);
    }
    return(0);
}



int main_testD_ascii_table(void)
{
    char c=' ';

    leds_init();
    uart0_initialize(uart_bps_9600);
    while (1)
    {
        leds_set(c);
        uart0_putc(c);
        _delay_ms(1000);
        c++; // warning - uint8_t should be used instead of char
        // because we intend to roll over at 128 or 256
        // and signed char behavior is undefined at that point
    }
    return(0);
}



int main(void)
{
    return( main_test6_getc_echo() ); // replace with tests 1, 2, 3, ... C, D.
}
