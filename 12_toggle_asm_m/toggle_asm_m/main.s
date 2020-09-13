#include <avr/io.h> #include "global_defs.inc" .GLOBAL main.EQU LED_alive , 5.EQU LED0 , 0.EQU LED1 , 1.EQU KEY0 , 4 ; note that "KEY0: is on the 4th PIN!.EQU KEY1 , 5.DATAled_status:    .BYTE 1key_last:    .BYTE 1.TEXTmain:main_initialize:;   setting LED "alive" and the lowest LED bit to be outputs;   other pins kept as inputs as they were originally
    in      R16,                        _SFR_IO_ADDR(DDRB)
    ori     R16,                        0b00111111
    out     _SFR_IO_ADDR(DDRB),         R16
;   setting LED "alive" and the lowest LED bit to be initially ON    in      R16,                        _SFR_IO_ADDR(PORTB)
    andi    R16,                        0b11000000
    ori     R16,                        (1<<LED_alive | 1<<LED0)
    out     _SFR_IO_ADDR(PORTB),        R16

    ; set pin 4 of port D to input with a pull-up resistor
    ; other pins kept as inputs as they were originally
    in      R16,                        _SFR_IO_ADDR(DDRD)
    andi    R16,                        0b00001111
    out     _SFR_IO_ADDR(DDRD),         R16
    in      R16,                        _SFR_IO_ADDR(PORTD)
    ; activates pull up resistors on used pin(s)
    ori     R16,                        1<<KEY0    out     _SFR_IO_ADDR(PORTD),        R16
;   Initialize variables and registers    ldi     R16,                        0    sts     led_status,                 R16    sts     key_last,                   R16main_superloop:    ; reading the button status(es)    in      R16,        _SFR_IO_ADDR(PIND)    lds     R18,        key_last    eor     R18,        R16    sts     key_last,   R16    ; start updating LEDs with preserving the unused bits    in      R16,        _SFR_IO_ADDR(PORTB)    sts     led_status, R16    ; blinking the "alive" LED    ldi     R17,        1<<LED_alive    lds     R16,        led_status    eor     R16,        R17    sts     led_status, R16    ; take care of button KEY0 -> LED0check_b0:    mov     R16,        R18    andi    R16,        1<<KEY0    breq    skip_b0    lds     R16,        key_last    andi    R16,        1<<KEY0    breq    skip_b0toggle_led0:    ldi     R17,        1<<LED0    lds     R16,        led_status    eor     R16,        R17    sts     led_status, R16skip_b0:    ; writing the updated LED output    lds     R16,        led_status    out     _SFR_IO_ADDR(PORTB),    R16    ; call delay function with number of MS in R3    ldi     R16,        100    mov     R3,         R16    rcall   delay_in_ms    ; note: perhaps it would be better to design the function to receive its parameter in R16 or above    ; as that would allow to set constant parameter with just one instruction instead of two?    rjmp    main_superloop