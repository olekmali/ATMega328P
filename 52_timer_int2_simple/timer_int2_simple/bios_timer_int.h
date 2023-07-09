//* Timer1 interrupt-based library - header file *
#ifndef BIOS_Timer1_INT_H_
#define BIOS_Timer1_INT_H_
#include <stdint.h>

#ifndef _PRESCALER_DEFS
#define _PRESCALER_DEFS
    #define timer_prescale_1    (1)
    #define timer_prescale_8    (2)
    #define timer_prescale_64   (3)
    #define timer_prescale_256  (4)
    #define timer_prescale_1024 (5)
#endif


void    Timer1_initialize   ( uint32_t frequency, void (*handle_fn)(void), uint8_t prescaler );
void    Timer1_shutdown     ();


#endif /* BIOS_Timer1_INT_H_ */
