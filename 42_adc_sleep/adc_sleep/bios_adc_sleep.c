//* bios_adc.c *
#include "bios_adc_sleep.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

static volatile uint16_t adc_result = 0;

void adc_init_wslepp (void)
{
    ADCSRA  |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)  | (1<<ADIE);  // Set ADC clock to 125kHz (==16MHz/128), enable ADC complete interrupt
    ADMUX    = 0;                   // for now let us use the very 1st channel
    ADMUX   |= (1 << REFS0);        // use VCC as Vref         (==AVCC==5V)
    ADMUX   |= (1 << ADLAR);        // left justify the result so that 8bits can be read from the high register
                                    // this works well if you do not need to have a 4bit cushion to prevent overflow on computations
    
    set_sleep_mode(SLEEP_MODE_ADC); // or   MCUCR |= (1 << SM0);
    ADCSRA  |= (1 << ADEN);         // start ADC
}


void adc_shutdown (void)
{
    ADCSRA = 0;
    ADMUX = 0;
}

void adc_select_ch (uint8_t channel)
{
    ADMUX = ( ADMUX & 0xE0 ) | ( 0x1F & channel );
}


uint8_t adc_get8b (void)
{
    // adc_result = 0; needed only to verify if sleep and interrupt really works
    sleep_enable();                     // or   MCUCR  |= (1 << SE);
    // conversion starts automatically when sleep mode is activated below
    sleep_cpu();                        // or   __asm volatile ("sleep");
    sleep_disable();                    // or   MCUCR &= ~(1<<SE);
    return( ( adc_result>>8 ) & 0xFF );
}


uint16_t adc_get10b (void)
{
    // adc_result = 0; needed only to verify if sleep and interrupt really works
    sleep_enable();                     // or   MCUCR  |= (1 << SE);
    // conversion starts automatically when sleep mode is activated below
    sleep_cpu();                        // or   __asm volatile ("sleep");
    sleep_disable();                    // or   MCUCR &= ~(1<<SE);
    return( adc_result );
}


uint16_t adc_get12b (void)
{
    uint16_t sum = 0;                   // Note: resolution increase works only when noise at level of the last ADC bit is present!
    for (uint8_t i=0; i<16; ++i)        // one needs to use 2^n iterations to increase the accuracy by n-bits,
        sum += adc_get10b() >> 6;       // let's add up right aligned results to allow 
    sum = sum << 2;                     // ( sum /16 ) << 6  results in ( sum >> 4 ) << 6 which is ( sum << 2 )
    return( sum );                      // return the average - n=4 yields sum/16, the last 2bits may still contain noise though
}


ISR(ADC_vect)
{
    adc_result = ADC;    
    MCUCR &= ~(1<<SE);                  // clear enable sleep
}
