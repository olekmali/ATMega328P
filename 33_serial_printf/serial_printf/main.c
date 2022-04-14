//* testing USART0 - main.c *
#include "bios_uart0.h"
#include "stdio_helper.h"

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>



void chomp(char* s)
{   // truncate the trailing end line character(s) if any
    while (*s!='\0' && *s!='\r' && *s!='\n') ++s;
    *s = '\0';
}


int main(void)
{
    uart0_initialize(uart_bps_9600);
    std_streams_initialize();

    while (1)
    {
        char buffer[16];

        fputs("What is your name please? ", stdout);
        fgets(buffer, sizeof(buffer), stdin);
        chomp(buffer);
        fprintf(stdout, "Hello %s!\r\nI hope that you are enjoying micro controllers\n", buffer);
        fprintf(com1, "Testing com1 stream...\n");
    }
    return(0);
}
