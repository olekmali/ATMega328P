//* Timer1 interrupt-based library - header file *
#ifndef BIOS_TIMER_PERIOD_H_
#define BIOS_TIMER_PERIOD_H_
#include <stdint.h>

// Timer1 capture input is PD.4

void    Timer1_capture_initialize   (void);
void    Timer1_shutdown     (void);

uint32_t getTime_Period     (void);
uint32_t getTime_High       (void);

uint8_t  isTimingReady      (void);
void     resetTimingReady   (void);


#endif /* BIOS_TIMER_PERIOD_H_ */
