//* External interrupt-based library - header file *
#ifndef BIOS_EXTERNAL_INT_H_
#define BIOS_EXTERNAL_INT_H_
#include <stdint.h>

void    External_Int0_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int0_shutdown  ();

void    External_Int1_initialize( uint8_t mode, void (*handle_fn)(void) );
void    External_Int1_shutdown  ();

enum eint_mode {
    EXT_INT_MODE_pin_low = 0,
    EXT_INT_MODE_pin_hi  = 1,
    EXT_INT_MODE_pin_falle = 2,
    EXT_INT_MODE_pin_raise = 3
     };

void    External_Int0_chg_mode  (uint8_t mode);
void    External_Int1_chg_mode  (uint8_t mode);

// PD2 is ExtInt0
// PD3 is ExtInt1


#endif /* BIOS_EXTERNAL_INT_H_ */
