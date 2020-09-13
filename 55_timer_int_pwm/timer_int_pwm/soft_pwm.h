//* soft_pwm.h *
#ifndef SOFT_PWM_H_
#define SOFT_PWM_H_
#include <stdint.h>

#define PWM_100_PERCENT 100
        // what value passed to PWM_generator_setParam(...) is considered 100%

#define PWM_RESOLUTION  10
        // this is the number of levels, e.g. 5 means 0%, 20%, 40%, 60%, 80%, and 100%
        // it is actually 1./resolution as 100 will yield 1% resolution of PWM

typedef uint8_t pwm_cnt_t;
        // this typedef works with PWM_RESOLUTION<=255 and PWM_100_PERCENT<=255

void        PWM_generator_interrupt (void);
void        PWM_generator_setParam  (uint8_t channel, pwm_cnt_t percentage);

#define PWM_CH_MAX      2
        // how many bits of the LED SFR are used for PWM

#define PWM_CH_MASK     0b00000011
        // allows state of bits or LEDs above PWM_CH_MAX to be preserved instead of set to 0



#endif /* SOFT_PWM_H_ */
