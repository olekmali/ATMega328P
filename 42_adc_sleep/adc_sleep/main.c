//* testing USART0 - main.c *
#include "bios_adc_sleep.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <stdlib.h>
#include <avr/interrupt.h>


int main(void)
{
    leds_init();
    keys_init();
    adc_init_wslepp();
    adc_select_ch(0);
    sei();  // enable interrupts - interrupt wakes up the MCU from sleep while reading ADC
    while (1)
    {
        if ( keys_get() & B_K7 ) {
            adc_select_ch(ADC_CH4);
            } else {
            adc_select_ch(ADC_CH5);
        }

        if ( keys_get() & B_K4 ) {
            // display a progress bar in logarithmic scale
            uint8_t val = adc_get8b();
            uint8_t disp= 0;
            while ( val>0 ) {
                disp = ( disp << 1 ) | 1 ;
                val >>= 2; // i.e. val = val >> 2; or val = val/4
            }
            leds_set( disp );
            } else {
            if ( keys_get() & B_K6 ) {
                // display only the 6LSB of 10 to check for noise
                leds_set( (uint8_t)( adc_get10b()>> 6 ) );
                } else {
                // display only the 6MSB
                leds_set( adc_get8b()>>2 );
            }
        }




    }
    return(0);
}
