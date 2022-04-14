//* testing Watchdog - main.c *
#include "bios_pwmdac.h"
#include <stdint.h>


int main(void)
{
    Timer2_PWM_initialize();
    Timer2_PWM_setDuty(0x3F); // 25% change and observe PortB.7

    while(1)
    {
        // Note: this example is incomplete
    }

    return(0);
}


