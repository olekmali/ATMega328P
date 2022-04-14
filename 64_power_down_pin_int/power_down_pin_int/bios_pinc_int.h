//* Pin change interrupt-based library - header file *
#ifndef BIOS_PINCINT_H_
#define BIOS_PINCINT_H_
#include <stdint.h>

void    PCINT_PINC_initialize( uint8_t which_pins, void (*handle_fn)(void) );
void    PCINT_PINC_shutdown  ();

#endif /* BIOS_PINCINT_H_ */
