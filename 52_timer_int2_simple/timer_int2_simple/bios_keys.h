//* bios_keys.h *
#ifndef BIOS_KEYS_H_
#define BIOS_KEYS_H_
#include <stdint.h>

#define B_K7 (0x08)
#define B_K6 (0x04)
#define B_K5 (0x02)
#define B_K4 (0x01)

void    keys_init       (void);
void    keys_shutdown   (void);
uint8_t keys_get        (void);

#endif /* BIOS_KEYS_H_ */