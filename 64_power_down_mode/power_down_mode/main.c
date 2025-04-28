//* testing TIMER1 interrupt - main.c *
#include "bios_timer_int.h"
#include "bios_external_int.h"
#include "bios_leds.h"
#include "bios_keys.h"
#include <avr/interrupt.h>
#include <avr/sleep.h>


#define TIMER1_FRQ  (10)
#define TIMEOUT_MS  (10000)
#define COUNT_AWAKE ( 1L* TIMEOUT_MS * TIMER1_FRQ / 1000 )

volatile uint16_t countdown_to_sleep = COUNT_AWAKE;

void MySleepyTimerFN (void)
{
    static uint8_t count = 0;
    count++;
    if (64<=count)
        count = 0;

    // sleep count down
    if (0<countdown_to_sleep)
        countdown_to_sleep--;

    // to show some activity
    leds_set( ( leds_get() & 0b11000000 ) | count );
}


void MyExternalIntFN (void)
{
    // reset countdown_to_sleep each time there is a button activity
    countdown_to_sleep = COUNT_AWAKE;
    sleep_disable();
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

    Timer1_initialize(TIMER1_FRQ, &MySleepyTimerFN, timer_prescale_256);

    External_Int_Pins_Enable();
    External_Int0_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);
    External_Int1_initialize(EXT_INT_MODE_pin_low, MyExternalIntFN);

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);    // Note: this will not awake on Timer1 event
    // set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Note: this will awake on Timer1 event

    sei();  // enable global interrupts, all circuitry must be initialized prior to this event

    while(1)
    {
        // enter your functionality here
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
    }

    return(0);
}
