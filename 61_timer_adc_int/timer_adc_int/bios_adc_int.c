//* bios_adc_int.c *
#include "bios_adc_int.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint8_t  channel       = 0;
static volatile uint8_t  latest_results[6] = {0}; // reserve storage for all single channels
static volatile uint8_t  new_results   = 0;

void adc_initialize (void)
{
    ADCSRA  |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1<<ADIE);  // Set ADC clock to 125kHz (==16MHz/128), enable ADC complete interrupt
    ADMUX    = ADC_CH_MIN;          // for now let us use the very 1st channel
    ADMUX   |= (1 << REFS0);        // use VCC as Vref         (==AVCC==5V)
    ADMUX   |= (1 << ADLAR);        // left justify the result so that 8bits can be read from the high register
                                    // this works well if you do not need to have a 4bit cushion to prevent overflow on computations

    ADCSRA  |= (1 << ADEN);         // start ADC
}

void adc_shutdown (void)
{
    ADCSRA = 0;
    ADMUX = 0;
}

void adc_startRound  (void)
{
    channel = ADC_CH_MIN;
    ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel);
    ADCSRA |= (1 << ADSC);  // start conversion
}

ISR(ADC_vect)
{
    latest_results[channel] = ADCH;
    if( channel == (ADC_CH_MAX) )
    {
        new_results = 1;
    } else {
        channel++;
        ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel);
        ADCSRA |= (1 << ADSC);  // start conversion
    }
}

uint8_t adc_getRecent (uint8_t channel)
{
    uint8_t value;
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    value = latest_results[channel];
    SREG = sreg_save;                   // restore interrupts
    return(value);
}

uint8_t isAdcReady (void)
{
    return(new_results);
}

void resetAdcReady (void)
{
    new_results = 0;
}
