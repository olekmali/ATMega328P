//* bootmenu.c *
#include "bootmenu.h"
#include <avr/io.h>

uint8_t boottime_menu()
{
    // save PORTD configuration to restore it later
    uint8_t save_dir  = DDRD;
    uint8_t save_port = PORTD;

    DDRD  = 0x0F & DDRD;
    PORTD = 0xF0 | (PORTD & 0x0F);

    // wait say: 500uS for pull up resistors to recharge pins before reading them
    // note: The actual minimum is 3 cycles plus
    //       whatever to take to charge the parasitic C
    //       using pull up resistors
    for (volatile uint8_t i=100; i!=0; i--) ;

    uint8_t choice = ( (~PIND)>>4) & 0x0F;

    // restore PORTD original configuration
    DDRD  = save_dir;
    PORTD = save_port;

    return(choice);
}
