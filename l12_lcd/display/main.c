//* testing LCD display initialization and basic functionality *
#include <stdint.h>
#include "lab_lcd.h"

#define F_CPU (16000000UL)
#include <util/delay.h>

/* begin testing stub */
#include <avr/io.h>

#define L5 (1<<5)
inline void L5init()   { DDRB = DDRB | L5;    }
inline void L5off()    { PORTB = PORTB & ~L5; }
inline void L5on()     { PORTB = PORTB | L5;  }
inline void L5toggle() { PORTB = PORTB ^ L5;  }
inline void halt()     { while(1) ; }
/* end testing stub */


// this test allows you to do the final tweak of the delay function
int main(void)
{
    L5init();
    lcd_init();
//  halt();

    lcd_putchr('A');
//  halt();

    lcd_putstr(" Testing... ");
    _delay_ms(2000);
//  halt();

    lcd_gotoXY(1, 0);
    lcd_putstr("Second Line "); // Replace this with your name to get credit
    _delay_ms(2000);
//  halt();

    lcd_home();
    lcd_putstr("Home ");
    _delay_ms(2000);
//  halt();

    lcd_clear();
    lcd_putstr("Clear Screen ");
    _delay_ms(2000);
//  halt();

    lcd_clear();
    lcd_cursor(0);
    lcd_putstr("No cursor ");
    _delay_ms(2000);
//  halt();

    lcd_clear();
    lcd_cursor(1);
    lcd_putstr("Cursor On ");
    _delay_ms(2000);
    //  halt();

    lcd_clear();
    lcd_putstr("Loop: ");

    char c = ' ';
    while(1)
    {
        _delay_ms(1000);
        L5toggle();

        lcd_putchr(c);
        if (c<'z') c++; else c= ' ';
    }

    return(0);
}
