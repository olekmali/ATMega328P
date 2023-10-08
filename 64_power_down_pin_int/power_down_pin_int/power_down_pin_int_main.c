//* Simple test to sleep and test pin changed interrupt on PORTC *
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define F_CPU (16000000UL)
#include <util/delay.h>

#include "bios_pinc_int.h"
#include "bios_leds.h"

void MyInterruptPinChangedFunction() {
    // Note: this is called from an interrupt
    //       do not cause any significant delay
    sleep_disable(); // <-- important! or else program will fall asleep on exiting the interrupt
}


//------------------------------------------------------------------------------------
// HAL Functions for Port C pins
//------------------------------------------------------------------------------------

void setup_PC012() {
    DDRC =  DDRC  & 0b11111000; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins are input/zero
    PORTC = PORTC | 0b00000111; // don't touch the upper 5 pin setup, just make sure that the lowest 3 pins has pull up resistors/one

    // enable the interrupt on the lowest 3 pins of PORTC
    PCINT_PINC_initialize( 0b00000111, MyInterruptPinChangedFunction );   // enable pin change interrupt on PINC.0
}

uint8_t get_PC012() {
    return( PINC & 0b00000111);
    // Note: for the four pushbuttons we typically use that would be: return( ( PIND >> 4 ) & 0x0F );
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

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    //  set_sleep_mode(SLEEP_MODE_PWR_SAVE);

    sei(); // enable all interrupts
    while (1)
    {
        sleep_enable();
        sleep_cpu();

        {   // the code fragment below is for illustration purposes only
            // it indicates that the main loop is running
            uint8_t wakeup_reason = get_PC012();
            if ( ( wakeup_reason & 0b00000001 ) == 0 ) {
                // button on pin 0 was just pressed down 1->0
                // or is being pressed down while another button changed
                leds_set( leds_get() | B_L0 );
                _delay_ms(100);
                leds_set( 0 );
            } else {
                // any other button changed or pin 0 button released
                leds_set( leds_get() | B_L1 );
                _delay_ms(100);
                leds_set( 0 );
            }
        }

    }
}
