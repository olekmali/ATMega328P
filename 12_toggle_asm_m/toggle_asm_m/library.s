#include <avr/io.h> #include "global_defs.inc"; this function
;   - receives a unit8_t  variable in register [r15]
;   - causes time delay of that many milliseconds by wasting time and energy
;   - returns 0 in [r15]
;
.GLOBAL delay_in_ms.EQU    OneMS,  3999 ; started with an approximation and then used proportion to adjust to be clsoe to 1 ms
.TEXTdelay_in_ms:
    push    R16
    in      R16, _SFR_IO_ADDR(SREG)
    push    R16

    push    R17
    push    R18

ms_counter_loop:

    ; note -- we only need two registers but that would speed up the loop and mabye we would have to go back to three registers to keep the count
    ldi     R18,        hi8(OneMS)      ; or 0xFF & ( OneMS >> 8 )
    ldi     R17,        lo8(OneMS)      ; or 0xFF &   OneMS

delay_loop:
    ; here be one millisecond loop by unsigned short int countdown to zero
    subi    R17,        1
    sbci    R18,        0
    brcc    delay_loop

    dec     R15         ; note that C flag is not set, so we have limited choice of BRXX
    brne    ms_counter_loop

    pop     R18
    pop     R17

    pop     R16
    out     _SFR_IO_ADDR(SREG),   R16
    pop     R16
    ret