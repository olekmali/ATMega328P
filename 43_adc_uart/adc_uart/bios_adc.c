//* bios_adc.c *
#include "bios_adc.h"
#include <avr/io.h>

void adc_init (void)
{
    ADCSRA  |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);  // Set ADC clock to 125kHz (==16MHz/128)
    ADMUX    = 0;                   // for now let us use the very 1st channel
    ADMUX   |= (1 << REFS0);        // use VCC as Vref         (==AVCC==5V)
    ADMUX   |= (1 << ADLAR);        // left justify the result so that 8bits can be read from the high register
                                    // this works well if you do not need to have a 4bit cushion to prevent overflow on computations
    
    // ADCSRA |= (1 << ADIE);          we do not use ADC complete interrupt at this time
    ADCSRA  |= (1 << ADEN);         // start ADC
}


void adc_shutdown (void)
{
    ADCSRA = 0;
    ADMUX = 0;
}

void adc_select_ch (uint8_t channel)
{
    ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel);
}


uint8_t adc_get8b (void)
{
    ADCSRA |= (1 << ADSC);              // start conversion
    while ( !( ADCSRA & (1<<ADIF) ) ) ; // wait until the conversion is complete
    return(ADCH);
}

uint16_t adc_get10b (void)
{
    ADCSRA |= (1 << ADSC);              // start conversion
    while ( !( ADCSRA & (1<<ADIF) ) ) ; // wait until the conversion is complete
    return(ADC);                        // 16bit ADC or, otherwise, read ADCL first and then ADCH    
}


