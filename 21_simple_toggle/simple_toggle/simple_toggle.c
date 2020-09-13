# include <avr/io.h>
# include <stdint.h>

int main (void)
{
    // initialize LED output pins on lower 4 bits of Port B
    DDRB  = DDRB  | 0x0F;       // affect DDR only the lower pins - set to push-pull out/1
    PORTB = PORTB | 0x0F;       // affect only the 4 output pins by setting them to 1
    //  PORTB = PORTB & 0xF0;       // affect only the 4 output pins by setting them to 0


    // do it forever without any delay
    while (1)
    {
        // delay loop for about 100 ms if no interrupts
        for (volatile uint16_t counter=0; counter<57140; counter++) ;

        // read the LEDs, ignore the other pins
        uint8_t leds = PORTB & 0x0F;

        leds = leds ^ 0x0F; // perform exclusive OR on the 4 bits in order to flip them

        // set the LEDs without disturbing the state of the other pins
        PORTB = (PORTB & 0xF0) | (leds & 0x0F);
    }
    return(0);
}
