// * the effect of static electricity on floating inputs *
#include <avr/io.h>

.GLOBAL main
.TEXT
main:

main_initialize:
;   setting LED "alive" and the lowest LED bit to be outputs
;   other pins kept as inputs as they were originally
    in      R16,                        _SFR_IO_ADDR(DDRB)
    andi    R16,                        0b11000000                  ; preserve two highest bit direction
    ori     R16,                        0b00111111                  ; set up output pins for LEDs
    out     _SFR_IO_ADDR(DDRB),         R16
    ; no output pins to set, they will be set repetitively soon anyway

    ; let's do nothing to the input (by default it is an input) and observe


main_superloop:
    ; reading the button status(es)
    in      R16,    _SFR_IO_ADDR(PIND)
    lsr     R16
    lsr     R16
    lsr     R16
    lsr     R16
    in      R17,    _SFR_IO_ADDR(PORTB)
    andi    R17,    0b11000000
    or      R17,    R16
    out     _SFR_IO_ADDR(PORTB),    R17

    rjmp    main_superloop



; either expected by linker, equivalent to the "old style" .ORG 0
; or alternatively generated by the C/C++ compiler on its own accord
.TEXT
_start:
reset_vector:
    jmp start
; interrupt table would follow *** here *** if there are any interrupt services
; 32bits per entry, up to 34 entries

; this is the part of the loader program that initializes the stack etc.
.TEXT
start:
    ldi     R16,                    0xFF && (RAMEND>>8)
    out     _SFR_IO_ADDR(SPH),      R16
    ldi     R16,                    0xFF && RAMEND
    out     _SFR_IO_ADDR(SPL),      R16
    jmp     main