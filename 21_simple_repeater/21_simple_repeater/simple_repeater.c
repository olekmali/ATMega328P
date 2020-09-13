# include <avr/io.h>
# include <stdint.h>

int main (void)
{
    // initialize LED output pins on lower 4 bits of Port B
    DDRB  = DDRB  | 0x0F;       // affect DDR only the lower pins - set to push-pull out/1
    PORTB = PORTB | 0x0F;       // affect only the 4 output pins by setting them to 1
    //  PORTB = PORTB & 0xF0;       // affect only the 4 output pins by setting them to 0

    // initialize BUTTON input pins on upper 4 bits of Port D
    DDRD  = DDRD  & 0x0F;       // affect DDR only the 4 upper pins - set to in/0
    PORTD = PORTD | 0xF0;       // affect pull up resistors only the 4 upper pins

    // do it forever without any delay
    while (1)
    {
        // read the buttons, ignore other pins, indicate pushed as 1 instead of 0
        uint8_t pins = ((~PIND)>>4) & 0x0F;

        uint8_t leds = pins;

        // set the LEDs without disturbing the state of the other pins
        PORTB = (PORTB & 0xF0) | (leds & 0x0F);

        // The above can be done in one line that is optimized
        // PORTB = (PORTB & 0xF0) | ( (~PIND>>4) & 0xF);
    }
    return(0);
}
