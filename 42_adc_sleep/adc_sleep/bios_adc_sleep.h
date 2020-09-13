//* bios_adc.h *
#ifndef BIOS_ADC_H_
#define BIOS_ADC_H_
#include <stdint.h>

void        adc_init_wslepp (void);
void        adc_shutdown    (void);

void        adc_select_ch   (uint8_t channel);

uint8_t     adc_get8b       (void);
uint16_t    adc_get10b      (void);
uint16_t    adc_get12b      (void);

#define ADC_CH0     (0)
#define ADC_CH1     (1)
#define ADC_CH2     (2)
#define ADC_CH3     (3)
#define ADC_CH4     (4)
#define ADC_CH5     (5)
#define ADC_CH6     (6)
#define ADC_CH7     (7)

#endif /* BIOS_ADC_H_ */