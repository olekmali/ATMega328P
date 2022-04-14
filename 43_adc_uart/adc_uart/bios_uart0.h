//* USART0 library - header file - 2019 *
#ifndef BIOS_UART0_H_
#define BIOS_UART0_H_
#include <stddef.h>     // needed for size_t
#include <stdint.h>     // needed for int8_t


// typical baud rates:
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


// allowed uart_stop_mode values:
#ifndef _SER_STOP
#define _SER_STOP
#define uart_stop_one (0)
#define uart_stop_two (1)
#endif


// allowed uart_parity_mode values:
#ifndef _SER_PARITY
#define _SER_PARITY
#define uart_parity_none (0)
#define uart_parity_even (2)
#define uart_parity_odd  (3)
#endif


void    uart0_initialize    (uint16_t baud);
void    uart0_initialize3   (uint16_t baud, uint8_t uart_stop_mode, uint8_t uart_parity_mode);
void    uart0_shutdown      ();

uint8_t uart0_ready_TX      ();
void    uart0_putc          (char c);
void    uart0_puts          (const char* const s);

uint8_t uart0_ready_RX      ();
char    uart0_getc          ();
char    uart0_getc_echo     ();
uint8_t uart0_check_error   (); // 0 if last char received OK

size_t  uart0_gets          (char* s, size_t size);
size_t  uart0_gets_echo     (char* s, size_t size);
size_t  uart0_gets_edit     (char* s, size_t size);

// binary operations
size_t uart0_read          (void* s, size_t size);
size_t uart0_write         (const void* const s, size_t size);

#endif /* BIOS_UART_H_ */
