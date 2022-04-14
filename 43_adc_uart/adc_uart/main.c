//* testing ADC and USART0 - main.c *
#include "bios_adc.h"
#include "bios_uart0.h"

#define F_CPU (16000000UL)
#include <util/delay.h>


#include <stdlib.h>


int main(void)
{
    uart0_initialize(uart_bps_9600);
    adc_init();
    // Note: ADC results are returned left aligned: 0b dddd dddd dd00 0000
    while (1)
    {
        adc_select_ch(ADC_CH4);
        uint16_t ch4 = adc_get10b()>> 6;

        adc_select_ch(ADC_CH5);
        uint16_t ch5 = adc_get10b()>> 6;

        // Convert to actual measured values
        uint16_t v4 = ( ch4 * 5000UL ) / 0x03FF;
        uint16_t v5 = ( ch5 * 5000UL ) / 0x03FF;

        /*
            // what happens when no typecasting? -- rollover at 65535 / 0x03FF
            uint16_t v4 = ( ch4 * 5000 ) / 0x03FF;
            uint16_t v5 = ( ch5 * 5000 ) / 0x03FF;

            // typecasting to ensure no out of range rollover
            uint16_t v4 = ( ch4 * 5000UL ) / 0x03FF;
            uint16_t v5 = ( ch5 * 5000UL ) / 0x03FF;

            // what happens when floating point is used?
            uint16_t v4 = (uint16_t) ( ch4 * ( 5000.0f / 0x03FF ) );
            uint16_t v5 = (uint16_t) ( ch5 * ( 5000.0f / 0x03FF ) );
        */

        // Convert to char string for transmission
        char buffer[10]; // make sure that the buffer size is large enough
        itoa(v4, buffer, 10);
        uart0_puts("V4 = ");
        uart0_puts(buffer);
        uart0_puts("mV\tV5 = ");
        itoa(v5, buffer, 10);
        uart0_puts(buffer);
        uart0_puts("mV   \r\n");

        _delay_ms(1000);
    }
    return(0);
}
