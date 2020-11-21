//* bios_adc_int.h *
#ifndef BIOS_ADC_INT_H_
#define BIOS_ADC_INT_H_
#include <stdint.h>

void            adc_initialize  (void);
void            adc_shutdown    (void);
void            adc_startRound  (void);
uint8_t         adc_getRecent   (uint8_t channel);

// semaphore communication with main loop
uint8_t         isAdcReady      (void);
void            resetAdcReady   (void);

// mux codes for each channel
#define ADC_CH0     0
#define ADC_CH1     1
#define ADC_CH2     2
#define ADC_CH3     3
#define ADC_CH4     4
#define ADC_CH5     5

// number of channels used at the moment
// ** edit these two constants below as needed **
#define ADC_CH_MIN  ADC_CH0
#define ADC_CH_MAX  ADC_CH5
// ** edit these two constants above as needed **

#endif /* BIOS_ADC_INT_H_ */
