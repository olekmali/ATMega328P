//* bios_leds.h *
#ifndef BIOS_LEDS_H_
#define BIOS_LEDS_H_
#include <stdint.h>

#define B_L0 (0x01)
#define B_L1 (0x02)
#define B_L2 (0x04)
#define B_L3 (0x08)
#define B_L4 (0x10)
#define B_L5 (0x20)

void    leds_init       (void);
void    leds_shutdown   (void);
uint8_t leds_get        (void);
void    leds_set        (uint8_t number);

#endif /* BIOS_LEDS_H_ */