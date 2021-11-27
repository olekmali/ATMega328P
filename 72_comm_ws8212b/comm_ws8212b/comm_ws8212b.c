//* Test program for the Neopixel WS8212B library *
#define F_CPU 16000000UL
#include <util/delay.h>
#include "lib_ws8212.h"

#include <stdlib.h> // for random number generator

#define ntests 5U
#define nneopx 2U
const rgb_t Test[ntests][nneopx] = {
    { {255,   0,   0}, {  0, 255,   0} },
    { {  0, 255,   0}, {  0,   0, 255} },
    { {  0,   0, 255}, {255,   0,   0} },
    { {255, 255, 255}, {255, 255, 255} },
    { {  0,   0,   0}, {  0,   0,   0} }
};

rgb_t Neopixel[nneopx] =  { 0 };

int main(void)
{
    ws8212_initialize();

    //Power on test
    for (uint8_t i=0; i<ntests; i++) {
        ws8212_program(Test[i], nneopx);
        _delay_ms(2000);
    }

    // srand(123);
    while (1)
    {
        for (size_t i=0; i<nneopx; i++) {
            Neopixel[i].r= (uint8_t)( rand() & 0x7F);
            Neopixel[i].g= (uint8_t)( rand() & 0x7F);
            Neopixel[i].b= (uint8_t)( rand() & 0x7F);
        }
        ws8212_program(Neopixel, nneopx);
        _delay_ms(500);
    }
}

