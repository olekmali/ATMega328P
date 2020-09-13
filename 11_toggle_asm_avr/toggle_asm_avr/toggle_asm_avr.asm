// * blink2 program *
.INCLUDE "m328Pdef.inc"


.EQU LED_alive = 5
.EQU LED0 = 0
.EQU LED1 = 1
.EQU KEY0 = 4 ; note that "KEY0: is on the 4th PIN!.EQU KEY1 = 5

.CSEG
.ORG $0000
reset_vector:
    jmp start


.DSEG
.ORG $0100

; what was sent to the LED output port previously, or what is about to be sent there
led_status:
    .BYTE 1
; previous status of push buttons saved so that any difference can be detected
key_last:
    .BYTE 1

.CSEG
start:
    ldi     R16,        high(RAMEND)
    out     sph,        R16
    ldi     R16,        low(RAMEND)
    out     spl,        R16

;   Initialize global variables in RAM
    eor     R16,        R16     ; you may want to start with a different value in case of HW, use ldi then
    sts     led_status, R16
;   eor     R16,        R16     ; no need to reset to 0 again
    sts     key_last,   R16

main_initialize:

main:
;   setting LED "alive" and the lowest LED bit to be outputs;   other pins kept as they were originally
    in      R16,                        DDRB
    ori     R16,                        0b00111111
    out     _SFR_IO_ADDR(DDRB),         R16
;   setting LED "alive" and the lowest LED bit to be initially ON    in      R16,                        PORTB
    andi    R16,                        0b11000000
    ori     R16,                        (1<<LED_alive | 1<<LED0)
    out     _SFR_IO_ADDR(PORTB),        R16

;   set pin 4 of port D to input with a pull-up resistor
;   other pins kept as inputs as they were originally
    in      R16,                        DDRD
    andi    R16,                        ~(1<<KEY0)  ; == ~ 0b00010000 ==  0b11101111
    out     _SFR_IO_ADDR(DDRD),         R16
    in      R16,                        PORTD
;   activates pull up resistors on used pin(s)
    ori     R16,                        1<<KEY0     ; == 0b00010000    out     _SFR_IO_ADDR(PORTD),        R16
;   Initialize variables and registers    ldi     R16,                        0    sts     led_status,                 R16    sts     key_last,                   R16
main_superloop:
; reading the button status(es)    in      R16,        PIND    lds     R18,        key_last    eor     R18,        R16    sts     key_last,   R16; start updating LEDs with preserving the unused bits    in      R16,        PORTB    sts     led_status, R16; blinking the "alive" LED    ldi     R17,        1<<LED_alive    lds     R16,        led_status    eor     R16,        R17    sts     led_status, R16; take care of button KEY0 -> LED0check_b0:    mov     R16,        R18    andi    R16,        1<<KEY0    breq    skip_b0    lds     R16,        key_last    andi    R16,        1<<KEY0    breq    skip_b0toggle_led0:    ldi     R17,        1<<LED0    lds     R16,        led_status    eor     R16,        R17    sts     led_status, R16skip_b0:; writing the updated LED output    lds     R16,        led_status    out     PORTB,      R16    ; call delay function with number of MS in R3    ldi     R16,        100    mov     R3,         R16    rcall   delay_in_ms; note: perhaps it would be better to design the function to receive its parameter in R16 or above
; as that would allow to set constant parameter with just one instruction instead of two?
    rjmp    main_superloop
; this function
;   - receives a short int variable in registers [r2] and returns 0 in [r2]
;   - causes time delay of that many milliseconds by wasting time and energy
;
.EQU OneMS = 16*1000/5

delay_in_ms:
    push    R16
    in      R16,   sreg
    push    R16

    .def    d0  = R17
    .def    d1  = R18
    .def    d2  = R19

    push    d0
    push    d1
    push    d2

delay_count:
    ldi     d2,     byte3 (OneMS)
    ldi     d1,     high  (OneMS)
    ldi     d0,     low   (OneMS)

delay_loop:
    subi    d0,     1
    sbci    d1,     0
    sbci    d2,     0
    brcc    delay_loop

    dec     R2
    brne    delay_count

    pop     d2
    pop     d1
    pop     d0

    pop     R16
    out     sreg,   R16
    pop     R16
    ret
