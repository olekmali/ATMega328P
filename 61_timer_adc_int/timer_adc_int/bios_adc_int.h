//* bios_adc_int.h *
#ifndef BIOS_ADC_INT_H_
#define BIOS_ADC_INT_H_
#include <stdint.h>

void            adc_initialize  (void);
void            adc_shutdown    (void);
void            adc_startRound  (void);
uint8_t         adc_getRecent   (uint8_t channel);

// number of channels used at the moment
#define ADC_CNT     (6)

// mux code for a channel
#define ADC_CH0     (0)
#define ADC_CH1     (1)
#define ADC_CH2     (2)
#define ADC_CH3     (3)
#define ADC_CH4     (4)
#define ADC_CH5     (5)

uint8_t  isAdcReady         (void);
void     resetAdcReady      (void);


inline uint8_t  adc_getChCount() { return(ADC_CNT); }

#endif /* BIOS_ADC_INT_H_ */
