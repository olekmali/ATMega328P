//* testing powerdown sleep with interrupts - main.c *
#include "bios_external_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

#define COUNT_AWAKE 16

volatile uint8_t countdown_to_sleep = COUNT_AWAKE;

void MyExternalIntFN (void)
{
    // reset countdown_to_sleep each time there is a button activity
    countdown_to_sleep = COUNT_AWAKE;
    sleep_disable(); // <-- important! or else program will fall asleep on exiting the interrupt
}

void External_Int_Pins_Enable() {
    DDRD = DDRD  & ~0b00001100;
    PORTD = PORTD | 0b00001100;
}


int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    External_Int_Pins_Enable();
    External_Int0_initialize(EXT_INT_MODE_pin_falle, &MyExternalIntFN);
    External_Int1_initialize(EXT_INT_MODE_pin_low,   &MyExternalIntFN);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Note: this will not awake on a Timer event
    // set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Note: this will awake on s Timer event

    sei();  // enable global interrupts, all circuitry must be initialized prior to this event
    while(1)
    {
        // enter your functionality here as needed
        // and remember to reset countdown_to_sleep each time there is a user activity of some kind

        // START make the if statement and sleep_enable one atomic operation
        cli();                  // clear all interrupts
        if (0==countdown_to_sleep)
        {
            sleep_enable();
            sei();              // STOP1 enable prior interrupts - make the if statement and sleep_enable one atomic operation
            leds_set(0);
            sleep_cpu();
            sleep_disable();
        }
        sei();                  // STOP2 enable prior interrupts - make the if statement and sleep_enable one atomic operation

        // enter your functionality here as needed
        if ( keys_get()!=0 ) countdown_to_sleep =COUNT_AWAKE;
        leds_set(countdown_to_sleep);
        countdown_to_sleep--;
        _delay_ms(200);
    }

    return(0);
}
