//* USART0 interrupt-based library - header file *
#ifndef BIOS_UART0_INT_H_
#define BIOS_UART0_INT_H_
#include <stdint.h>


void    uart0_initialize    ( uint16_t baud, void (*handle_rx)(char) );
void    uart0_shutdown      ();

// This example shows only data item received interrupt
// Your function must be as short as possible, preferably without any loops

// This example will send data the classic way for simplicity
uint8_t uart0_ready_TX      ();
void    uart0_putc          (char c);
void    uart0_puts          (const char* const s);


#ifndef _SER_BOUD
#define _SER_BOUD
    #define uart_bps_2400      2400
    #define uart_bps_4800      4800
    #define uart_bps_9600      9600
    #define uart_bps_19200    19200
    #define uart_bps_38400    38400
    #define uart_bps_57600    57600
    #define uart_bps_115200  115200
#endif


#endif /* BIOS_UART_INT_H_ */
