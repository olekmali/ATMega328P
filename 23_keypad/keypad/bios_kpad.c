//* bios_kpad.c *
#include "bios_kpad.h"
#include <avr/io.h>

#define KEY_BOUNCE_CNT (5)

inline void delay_digital_io_change(void)
{
    // Note: asm("nop"); - this would work too but that is not ANSI C90 or C99 standard notation
    __asm__ __volatile__("nop");
    __asm__ __volatile__("nop");
    __asm__ __volatile__("nop");
    // Possible test question: what is inline modifier doing?
    // Possible test question: does inline work both in Debug and Release compiler mode?
    // Possible test question: what is volatile modifier doing?
    // Possible test question: does volatile work both in Debug and Release compiler mode?
}

// Row and Column line manipulation
inline void setDDRcolumns(uint8_t v)    { DDRD  = (DDRD  & 0x0F) | ( (v<<4) & 0xF0); }
inline void setPORTcolumns(uint8_t v)   { PORTD = (PORTD & 0x0F) | ( (v<<4) & 0xF0); }
inline void setDDRrows(uint8_t v)   { DDRD  = (DDRD  & 0xF0) | ( v & 0x0F); }
inline void setPORTrows(uint8_t v)  { PORTD = (PORTD & 0xF0) | ( v & 0x0F); }
// Row and Column low-level actions
inline void rechargeAllLines()      { setDDRcolumns(0b1111); setPORTcolumns(0b1111); setDDRrows(0b1111); setPORTrows(0b1111); }
inline void disconnectAllLines()    { setDDRcolumns(0b0000); setPORTcolumns(0b0000); setDDRrows(0b0000); setPORTrows(0b0000); }
inline void SetReadRow()            { setDDRcolumns(0b0000); setPORTcolumns(0b1111); }
inline uint8_t getColumnsShifted()  { return( (PIND>>4) & 0x0F ); }

uint16_t scan_keypad()
{
    // save the status of the ports before running the keypad scan
    // this may be needed because we share some lines with other peripherals
    uint8_t s_ddrd = DDRD;
    uint8_t s_pord = PORTD;

    // The scanning sequence is as follows:
    // ROW DDR  ROW PORT    COL DDR  COL PORT
    // 1111     1111        1111    1111 -- initial charge of parasitic Cs
    // wait
    // 0000     1111        1000    0111 -- scan the 3rd row
    // wait, read, recharge
    // read row
    // 0000     1111        0100    1011 -- scan the 2nd row
    // wait, read, recharge
    // read row
    // 0000     1111        0010    1101 -- scan the 1st row
    // wait, read, recharge
    // read row
    // 0000     1111        0001    1110 -- scan the 0th row
    // wait, read, recharge
    // read row
    // 0000     0000        0000    0000 -- disconnect for PIN safety
 // or 0000     1111        1111    0000 -- for PIN change interrupt driven keypad


    uint16_t status = KEYPAD_NONE;

    // scan the third / bottom row, get all columns
    rechargeAllLines();
    delay_digital_io_change();
    SetReadRow(); setDDRrows(0b1000); setPORTrows(0b0111);
    delay_digital_io_change();
    status = (status<<4) | (0x0F & ~ getColumnsShifted() );

    // scan the second row
    rechargeAllLines();
    delay_digital_io_change();
    SetReadRow(); setDDRrows(0b0100); setPORTrows(0b1011);
    delay_digital_io_change();
    status = (status<<4) | (0x0F & ~ getColumnsShifted() );

    // scan the first row
    rechargeAllLines();
    delay_digital_io_change();
    SetReadRow(); setDDRrows(0b0010); setPORTrows(0b1101);
    delay_digital_io_change();
    status = (status<<4) | (0x0F & ~ getColumnsShifted() );

    // scan the zeroth / top row
    rechargeAllLines();
    delay_digital_io_change();
    SetReadRow(); setDDRrows(0b0001); setPORTrows(0b1110);
    delay_digital_io_change();
    status = (status<<4) | (0x0F & ~ getColumnsShifted() );

//  disconnectAllLines();

    // restoring the original port setup
    // note: these four lines may have to be set up differently for pin changed or edge interrupt mode in Junior Lab
    DDRD  = s_ddrd;
    PORTD = s_pord;

    return(status);
}

char keypressed(void)
{
    static const char matrix[16] = {    KEY_A, KEY_3, KEY_2, KEY_1,
                                        KEY_B, KEY_6, KEY_5, KEY_4,
                                        KEY_C, KEY_9, KEY_8, KEY_7,
                                        KEY_D, KEY_H, KEY_0, KEY_S
                                    };
    // Possible test question: what is the use for the lookup table above
    uint8_t  keycnt = 0;
    uint8_t  keymin = KEY_NONE;
    uint16_t keys = scan_keypad();
    if (keys!=KEYPAD_NONE) {
        for (uint8_t i=0; i<16; i++) {
            if ( keys & 0x01 /* !=0 */ ) {
                keymin=matrix[i];
                keycnt++;
            }
            keys = keys >> 1;
        }
    }
    if (keycnt<2)   return(keymin);
    else            return(KEY_MANY);
    // Possible test question: what happens if we always return(keymin) regardless of keycnt value?
}

char getkey(void)
{
    uint8_t key_current=KEY_NONE, key_old;
    uint8_t key_cnt=0;
    do {
        key_old     = key_current;
        key_current = keypressed();
        if (key_current==key_old) key_cnt++;
        else key_cnt=0;
    } while (key_current==KEY_NONE||key_current==KEY_MANY||key_cnt<KEY_BOUNCE_CNT);

    // wait for the key release
    // Challenge Question 1: How to implement simplified auto-repeat? (easy)
    // Challenge Question 2: How to implement auto-repeat with the first key wait longer,
    //              and subsequent shorter like in the real computer keyboard? (require static (hidden global) variable inside the function)
    while (KEY_NONE!=keypressed()) ;

    return(key_current);
}
