//* Library to communicate with WS8212B - implementation file *
#include "lib_ws8212.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#include <util/delay.h>

#define WS8212Bport     PORTC
#define WS8212Bddr      DDRC
#define WS8212Bpin      PINC
#define WS8212Bbit      (1<<3)

/*
 * Data sheet timing specifications for WS8212
 *
 * T0H high voltage time 0.4us  ( 6.4clk)
 * T1H high voltage time 0.8us  (12.8clk)
 * T0L low voltage time 0.85us  (13.6clk)
 * T1L low voltage time 0.45us  ( 7.2clk)
 * All timing above ±150ns
 * RES low voltage time >=50us
 * High bit sent first GRB: G76543210R76543210B76543210
 */

/*
 * Modified timing specifications according to reverse engineering reported by Josh posted in 2014/2020:
 * https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
 *
 * T0H high voltage time 0.35us ( 5.6clk)
 * T1H high voltage time 0.7us  (11.2clk)
 * Tlo low voltage time 0.6-5us ( 9.6clk)
 * RES low voltage time >=6us
 */



static inline void sendByte(uint8_t val)
{
    for (uint8_t i=8; i!=0; i--) {               // i-- 1clk, i!=0 2clk
        if ((val&0b10000000)==0 ) {             // 2 or 3 clk depending on branch
            WS8212Bport |= WS8212Bbit;          // 3 clk
            _delay_us(0.1); // reduced to account for timing of the next line
            // Note: Release mode optimizes more for mem size which changes timing of C code
            //      and delay above needs to be reduced from 0.3 to 0.1 which still works in Debug mode
            WS8212Bport &= ~WS8212Bbit;         // 3 clk ( 0.18us)
            _delay_us(1);   // just bare minimum as more time will be spent in loops
        } else {
            WS8212Bport |= WS8212Bbit;          // 3 clk
            _delay_us(0.5); // reduced to account for timing of the next line
            // Note: Release mode optimizes more for mem size which changes timing of C code
            //      and delay above is still above 0 bit timing abd below reset timing
            WS8212Bport &= ~WS8212Bbit;         // 3 clk ( 0.18us)
            _delay_us(1);   // just bare minimum as more time will be spent in loops
        }
        val=val<<1;                             // 1 clk
    }
}



void ws8212_initialize() {
    WS8212Bddr  |= WS8212Bbit;  // output
    WS8212Bport &= ~WS8212Bbit; // restart transmission
}

void ws8212_program(const rgb_t data[], uint8_t length) {
    _delay_us(50);      // to make sure that at least 50us passed since the previous programming or initialization
    uint8_t sreg=SREG;  //save SREG
    cli();              //clear interrupts
    for (uint8_t i=0; i<length; i++) {
        sendByte(data[i].g);
        sendByte(data[i].r);
        sendByte(data[i].b);
    }
    // line status is left as low which prompts restart of the programming mode
    SREG=sreg;                  //restore SREG (including interrupt status)
}

void ws8212_shutdown() {
    WS8212Bddr  &= ~WS8212Bbit;
    WS8212Bport &= ~WS8212Bbit; // Note: for low power sleep mode use "PORTC |= WS8212Bpin" instead
}
