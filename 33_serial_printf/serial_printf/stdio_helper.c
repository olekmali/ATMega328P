//* stdio_helper.c makes printf and scanf work *
#include "stdio_helper.h"
#include "bios_uart0.h"


int	avr_putchar(char c, FILE *stream)
{
    if ('\n'==c) uart0_putc('\r');
    uart0_putc(c);
    return(0);
}


int	avr_getchar(FILE *stream)
{
    static uint8_t newline = 0;
    // ^^^ static menas that the variable is actually global but it is visible only inside the current block {}
    // This variable is initialized ONLY ONCE to 0 and then it holds its value between the function is called

    if ( !newline )
    {
        char c = uart0_getc();
        if ('\r'== c ) newline = 1;
        uart0_putc(c);              // echo for user interaction
        return( (uint8_t)(c) );     // typecast to unsigned to ensure result >0
    } else {
        char c = '\n';
        newline = 0;
        uart0_putc(c);              // echo for user interaction
        return( (uint8_t)(c) );     // typecast to unsigned to ensure result >0
    }

}


FILE* com1;
// ^^^^^^^ this is the actual variable declaration


void std_streams_initialize()
{   // assign the custom defined putchar and getchar to standard input, output, and error streams
    static FILE uart0_ostream = FDEV_SETUP_STREAM(avr_putchar,  NULL,           _FDEV_SETUP_WRITE);
    static FILE uart0_istream = FDEV_SETUP_STREAM(NULL,         avr_getchar,    _FDEV_SETUP_READ);
    static FILE uart0_iostream= FDEV_SETUP_STREAM(avr_putchar,  avr_getchar,    _FDEV_SETUP_RW);
    stdin  = &uart0_istream;
    stdout = &uart0_ostream;
    stderr = &uart0_ostream;
    com1   = &uart0_iostream;
}


//* Note: *
/*
    Many small micro controller compilers have simplified STDIO library
    Those compilers require that you only define two functions to get
    gets, puts, scanf and printf working. They would not support files

    void putchar(char c);  _or_  char putchar(char c);
    int  getchar();


*/
