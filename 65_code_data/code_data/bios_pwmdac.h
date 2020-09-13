//* Timer2 hardware PWM - header file *
#ifndef BIOS_PWMDAC_H_
#define BIOS_PWMDAC_H_
#include <stdint.h>

void    Timer2_PWM_initialize   ();
void    Timer2_PWM_shutdown     ();

void    Timer2_PWM_setDuty      (uint8_t duty);
// Output is on PortB Pin 7

// The constants below are informative only. Changing them will have no effect on the library code
#define DAC_PWM_RESOLUTION  (256)
#define DAC_PWM_100_PERCENT (255)


#endif /* BIOS_PWMDAC_H_ */