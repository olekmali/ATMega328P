//* simplified keypad test - main.c *
#include <stdint.h>
#include "bootmenu.h"
#include "lib_kpads_test.h"



int main(void)
{
    const uint8_t mode = boottime_menu();

    if      ( 0B0000 == mode )  test0_getkey_value();
    // diagnostic behavior
    else if ( 0b1000 & mode  )  test8_keypressed_status();
    else if ( 0b0010 == mode )  test2_shift();
    else if ( 0b0001 == mode )  test1_count_up();
    else                        test0_getkey_value();
    return(0);
}
