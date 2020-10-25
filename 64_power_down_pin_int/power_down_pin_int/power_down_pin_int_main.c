//* Simple test to sleep and test pin changed interrupt on PORTC *
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#include "bios_pinc_int.h"
#include "bios_leds.h"

//------------------------------------------------------------------------------------
// HAL Functions
//------------------------------------------------------------------------------------
inline void sleep_On() {
    //  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    set_sleep_mode(SLEEP_MODE_PWR_SAVE);
    sleep_enable();
    sleep_cpu();
}

inline void sleep_Off() {
    sleep_disable();
}


//------------------------------------------------------------------------------------
// Pin Changed Interrupt Functionality
//------------------------------------------------------------------------------------
void MyInterruptPinChangedFunction () {
    // this is an example that just does something visible
    // replace with your application code as needed
    uint8_t t = leds_get();
    t = t ^ B_L0;
    leds_set( t );

}


void setup_PC012() {
    DDRC =  DDRC  & 0b11111000; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins are input/zero
    PORTC = PORTC | 0b00000111; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins has pull up resistors/one

    // enable the interrupt on the lowest 3 pins of PORTC
    PCINT_PINC_initialize( 0b00000111, MyInterruptPinChangedFunction );   // enable pin change interrupt on PINC.0
}

void setup_Unused_Pins() {
    // Port B - all pins are used
    // Pins --543210 are set as outputs
    // Pins 76------ are connected to oscillator circuitry
    //
    // Port C Pins --543 are unused
    // Pins -----210 are already set as inputs with pull-ups
    DDRC  &= 0b11000111; // set as inputs
    PORTC |= 0b00111000; // set with pull-up resistors to prevent "floating"
    //
    // Port D Pins 765432-- are unused
    // Pins ------10 are connected to UART-USB and thus are not floating
    DDRD  &= 0b00000011; // set as inputs
    PORTD |= 0b11111100; // set with pull-up resistors
}


int main(void)
{
    leds_init();
    setup_PC012();
    setup_Unused_Pins();

    sei(); // enable all interrupts
    while (1)
    {
      sleep_On();

        {   // the code fragment below is for illustration purposes only
            // it indicates that the main loop is running
            cli(); leds_set( leds_get() | B_L4 ); sei();
            for (volatile uint32_t i =0; i<30000; i++) ;
            cli(); leds_set( leds_get() & ~B_L4 ); sei();
            for (volatile uint32_t i =0; i<30000; i++) ;
        }

    }
}

