//* blink and toggle in c - main.c for test 0 *
#include "bios.h"
// #include "library.h"

// This test should work out of the box without your library
int main(void)
{
    init_digital_io();

    while(1)
    {
        set_leds( get_buttons() );
    }

    return(0);
}
