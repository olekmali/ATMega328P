//* blink and toggle in c - library.h *
#ifndef LIBRARY_H_
#define LIBRARY_H_
#include <stdint.h>

void    delay           ( unsigned short int milliseconds );

uint8_t check_button    ( uint8_t old_buttons, uint8_t new_buttons, uint8_t button_mask);

uint8_t toggle_bit      ( uint8_t old_bits, uint8_t bit_mask);

#endif /* LIBRARY_H_ */
