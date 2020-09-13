//* blink and toggle in c - bios.h *
#ifndef BIOS_H_
#define BIOS_H_
#include <stdint.h>

void    init_digital_io();

uint8_t get_buttons();

void    set_leds(uint8_t new_status);


#endif /* BIOS_H_ */
