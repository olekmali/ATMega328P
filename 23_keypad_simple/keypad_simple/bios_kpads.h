//* bios_kpads.h *
#ifndef BIOS_KPADS_H_
#define BIOS_KPADS_H_
#include <stdint.h>

/*  NOTE! NEW WIRING:
    Connect the LEDs to lower bits PortB (5 LEDs to bits 0 through 4)
    Connect the Buttons to PortD (4 keys to upper 4 bits 4 through 7)
    Connect the Keypad rows to upper four bits of PortD (same as buttons)
    Connect the Keypad columns to the lowest 4 bits of PortC
*/

// include the line below before #include "bioskpad.h" to use ASCII code
// #define KEY_ASCII 1

// *** keypressed *** return codes ***
#ifdef KEY_ASCII
    #define KEY_0           '0'
    #define KEY_1           '1'
    #define KEY_2           '2'
    #define KEY_3           '3'
    #define KEY_4           '4'
    #define KEY_5           '5'
    #define KEY_6           '6'
    #define KEY_7           '7'
    #define KEY_8           '8'
    #define KEY_9           '9'
    #define KEY_A           'A'
    #define KEY_B           'B'
    #define KEY_C           'C'
    #define KEY_D           'D'
    #define KEY_H           '#'
    #define KEY_S           '*'
    #define KEY_NONE        0x80
    #define KEY_MANY        0xFF
#else
    #define KEY_0           0
    #define KEY_1           1
    #define KEY_2           2
    #define KEY_3           3
    #define KEY_4           4
    #define KEY_5           5
    #define KEY_6           6
    #define KEY_7           7
    #define KEY_8           8
    #define KEY_9           9
    #define KEY_A           10
    #define KEY_B           11
    #define KEY_C           12
    #define KEY_D           13
    #define KEY_H           14
    #define KEY_S           15
    #define KEY_NONE        0b00010000
    #define KEY_MANY        0b00011111
#endif

#define KEY_HASH        (KEY_H)
#define KEY_ASTERISK    (KEY_S)

char     keypressed     (void);
char     getkey         (void);

#endif /* BIOS_KPADS_H_ */
