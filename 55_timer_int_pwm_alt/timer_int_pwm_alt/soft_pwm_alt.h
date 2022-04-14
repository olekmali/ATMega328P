//* soft_pwm.h *
#ifndef SOFT_PWM_ALT_H_
#define SOFT_PWM_ALT_H_
#include <stdint.h>

void        PWM_generator_interrupt (void);
void        PWM_generator_setFreq   (uint32_t int_frq, uint16_t pwm_frq);
void        PWM_generator_setCoef   (uint8_t channel, uint8_t percentage);

#define PWM_CH_MAX      6
        // how many bits of the LED SFR are used for PWM

#define PWM_CH_MASK     0b00111111
        // allows state of bits or LEDs above PWM_CH_MAX to be preserved instead of set to 0

#define PWM_100_PERCENT 100
        // what value passed to PWM_generator_setParam(...) is considered 100%

#endif /* SOFT_PWM_ALT_H_ */
