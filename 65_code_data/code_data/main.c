//* testing Watchdog - main.c *
#include "bios_leds.h"
#include "bios_keys.h"
#include "bios_timer_int.h"
#include "bios_pwmdac.h"

#include <stdint.h>

#include <avr/interrupt.h>

#include <avr/pgmspace.h>
// Must also read http://www.nongnu.org/avr-libc/user-manual/pgmspace.html


#define DEBUG_205   // comment or uncommencement depending if debug LED actions are desired


const int16_t SINE_TABLE[] PROGMEM = { // half - 128, full - 256
                        // ^^^^^^^ in code memory
    0x0000, 0x0324, 0x0647, 0x096a, 0x0c8b, 0x0fab, 0x12c8, 0x15e2,
    0x18f8, 0x1c0b, 0x1f19, 0x2223, 0x2528, 0x2826, 0x2b1f, 0x2e11,
    0x30fb, 0x33de, 0x36ba, 0x398c, 0x3c56, 0x3f17, 0x41ce, 0x447a,
    0x471c, 0x49b4, 0x4c3f, 0x4ebf, 0x5133, 0x539b, 0x55f5, 0x5842,
    0x5a82, 0x5cb4, 0x5ed7, 0x60ec, 0x62f2, 0x64e8, 0x66cf, 0x68a6,
    0x6a6d, 0x6c24, 0x6dca, 0x6f5f, 0x70e2, 0x7255, 0x73b5, 0x7504,
    0x7641, 0x776c, 0x7884, 0x798a, 0x7a7d, 0x7b5d, 0x7c29, 0x7ce3,
    0x7d8a, 0x7e1d, 0x7e9d, 0x7f09, 0x7f62, 0x7fa7, 0x7fd8, 0x7ff6,
    0x7fff, 0x7ff6, 0x7fd8, 0x7fa7, 0x7f62, 0x7f09, 0x7e9d, 0x7e1d,
    0x7d8a, 0x7ce3, 0x7c29, 0x7b5d, 0x7a7d, 0x798a, 0x7884, 0x776c,
    0x7641, 0x7504, 0x73b5, 0x7255, 0x70e2, 0x6f5f, 0x6dca, 0x6c24,
    0x6a6d, 0x68a6, 0x66cf, 0x64e8, 0x62f2, 0x60ec, 0x5ed7, 0x5cb4,
    0x5a82, 0x5842, 0x55f5, 0x539b, 0x5133, 0x4ebf, 0x4c3f, 0x49b4,
    0x471c, 0x447a, 0x41ce, 0x3f17, 0x3c56, 0x398c, 0x36ba, 0x33de,
    0x30fb, 0x2e11, 0x2b1f, 0x2826, 0x2528, 0x2223, 0x1f19, 0x1c0b,
    0x18f8, 0x15e2, 0x12c8, 0x0fab, 0x0c8b, 0x096a, 0x0647, 0x0324,
/*  and the negative numbers...
    0x0000, 0xfcdc, 0xf9b9, 0xf696, 0xf375, 0xf055, 0xed38, 0xea1e,
    0xe708, 0xe3f5, 0xe0e7, 0xdddd, 0xdad8, 0xd7da, 0xd4e1, 0xd1ef,
    0xcf05, 0xcc22, 0xc946, 0xc674, 0xc3aa, 0xc0e9, 0xbe32, 0xbb86,
    0xb8e4, 0xb64c, 0xb3c1, 0xb141, 0xaecd, 0xac65, 0xaa0b, 0xa7be,
    0xa57e, 0xa34c, 0xa129, 0x9f14, 0x9d0e, 0x9b18, 0x9931, 0x975a,
    0x9593, 0x93dc, 0x9236, 0x90a1, 0x8f1e, 0x8dab, 0x8c4b, 0x8afc,
    0x89bf, 0x8894, 0x877c, 0x8676, 0x8583, 0x84a3, 0x83d7, 0x831d,
    0x8276, 0x81e3, 0x8163, 0x80f7, 0x809e, 0x8059, 0x8028, 0x800a,
    0x8000, 0x800a, 0x8028, 0x8059, 0x809e, 0x80f7, 0x8163, 0x81e3,
    0x8276, 0x831d, 0x83d7, 0x84a3, 0x8583, 0x8676, 0x877c, 0x8894,
    0x89bf, 0x8afc, 0x8c4b, 0x8dab, 0x8f1e, 0x90a1, 0x9236, 0x93dc,
    0x9593, 0x975a, 0x9931, 0x9b18, 0x9d0e, 0x9f14, 0xa129, 0xa34c,
    0xa57e, 0xa7be, 0xaa0b, 0xac65, 0xaecd, 0xb141, 0xb3c1, 0xb64c,
    0xb8e4, 0xbb86, 0xbe32, 0xc0e9, 0xc3aa, 0xc674, 0xc946, 0xcc22,
    0xcf05, 0xd1ef, 0xd4e1, 0xd7da, 0xdad8, 0xdddd, 0xe0e7, 0xe3f5,
    0xe708, 0xea1e, 0xed38, 0xf055, 0xf375, 0xf696, 0xf9b9, 0xfcdc, 
*/
};




//------------------------------------------------------------------------------------
// Custom DATA TYPES
//------------------------------------------------------------------------------------
typedef enum WaveForm_t {               // the different possible output waveforms
OFF, SQUARE, SINE, TRIANGLE, SAW
} Waveform;

//------------------------------------------------------------------------------------
// Global CONSTANTS
//------------------------------------------------------------------------------------
#define SAMPLING__FRQ   (200000L)               // sampling frequency for controlling the DAC which is Timer interrupt
#define PHASE_PREC      (0xffff)                // range of phase accumulator (which is uint16_t)
#define PHASE_HALF      (PHASE_PREC>>1)         // half of range of phase accumulator
#define MAX_AMPLITUDE   (0xFF)                  // maximum amplitude for DAC (8bit PWM based DAC)

#define INIT_WAVE       (TRIANGLE)
#define INIT_FRQ        (1)
#define INIT_AMPLITUDE  (MAX_AMPLITUDE>>4)
#define INIT_OFFSET     (0)

#define KEY_SCAN_FREQUENCY  (50)

//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------

static volatile enum WaveForm_t output_waveform = INIT_WAVE;
static volatile uint16_t        frequency   = INIT_FRQ; // Nyquist says at least two samples per wave period, DraAM would say "use at least TEN samples!"
static volatile uint8_t         amplitude   = INIT_AMPLITUDE;
static volatile int8_t          offset      = 0;

// (6.28*X) is actually coded using integers only as (628*S/100)
static volatile uint16_t        phase_add   = (628L * INIT_FRQ * PHASE_PREC / SAMPLING__FRQ / 100);

void PatternGenerator(void)
{
    static uint16_t phase_acc    = 0;   // holds phase accumulator, Note: will roll over at 65536
    static uint8_t  next_DAC_out = 0;   // make sure this happens at the beginning of each interrupt to minimize jitter

    int8_t  temp1;              // the temporary value that passes
    // through 3 stages before being written to DAC1
    int16_t temp2;              // temp1 adjusted with current amplitude

    #ifdef DEBUG_205
        // Light LED.1 when the portion of the interrupt is serviced
        leds_set( leds_get() | 0x02 );
    #endif

    // do this at the beginning of the interrupt to minimize jitter caused by variable time needed to compute things
    Timer2_PWM_setDuty(next_DAC_out);   // change and observe PortB.7

    phase_acc += phase_add;     // increment phase accumulator

    #ifdef DEBUG_205
        // Flash LED.0 at the DAC signal frequency
        if ( phase_acc < PHASE_HALF )
        {
            leds_set( leds_get() | 0x01 );
        } else {
            leds_set( leds_get() & ~0x01 );
        }
    #endif


    // set the value of <temp1> to the next output of DAC at full-scale
    // amplitude; the  rails are +127, -128 (8bit PWM based DAC)
    switch (output_waveform)
    {
        case SINE:
        // read the table value
        if ( phase_acc < PHASE_HALF )
        {
            temp1 = pgm_read_word ( &SINE_TABLE[(phase_acc >> 8)] ) >>8;
            //      ^^^^^^^^ access from code memory (copy to data)
            //              ^^^^^ access that many bytes
            //                                             ^^^^ map 0..65535 to 0..255
            //                                                      ^^^ use only the high byte of the array because this is 8bit DAC
            //  temp1 = SINE_TABLE[phase_acc/256];  // [i=0..127]
        } else {
            temp1 = - pgm_read_word ( &SINE_TABLE[((phase_acc - PHASE_HALF) >> 8)] ) >>8;
            //      ^^^^^^^^ access from code memory (copy to data)
            //              ^^^^^ access that many bytes
            //                                                              ^^^^ map 0..65535 to 0..255
            //                                                                       ^^^ use only the high byte of the array because this is 8bit DAC
            // VAL[i=128..255] == -VAL[i-128]
            // Note: Ise -(expression) and never use *(-1). Otherwise, you are trusting the compiler optimizer too much!
        }
        break;

        case SQUARE:
        // if in the first half-period, then high
        if ( phase_acc < PHASE_HALF )
        {
            temp1 = 0xFF;
        } else {
            temp1 = 0;
        }
        break;

        case TRIANGLE:
        // in first half-period, then  y = mx + b
        if ( phase_acc < PHASE_HALF )
        {
            temp1 =  ((phase_acc << 1) - PHASE_HALF ) >> 8;
            // else, in the second half of period
        } else {
            temp1 = -((phase_acc << 1) + PHASE_HALF ) >> 8;
        }
        break;

        case SAW:
        temp1 = (phase_acc - PHASE_HALF) >> 8;
        break;

        case OFF:
        default:
        temp1 = 0;
        break;
    }

    // Adjust the Gain
    // Note: make sure that computations are carried on using 16-bit int16_t to prevent overflow
    temp2 = (int16_t)amplitude * (int16_t)temp1 ; 
    temp1 = temp2 >> 8;                 // get just the upper byte
    
    next_DAC_out = temp1 + offset;
    

    #ifdef DEBUG_205
        // Light LED.1 when the portion of the interrupt is serviced
        leds_set( leds_get() & ~0x02 );
    #endif    
}


void DAC0_Timer3_SetFrequency(uint16_t newfrequency)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    if (newfrequency<1) newfrequency=1;
    frequency = newfrequency;
    phase_add   = (628L * frequency * PHASE_PREC / SAMPLING__FRQ / 100);
    SREG = sreg_save;                   // restore interrupts
}

void DAC0_Timer3_SetAmplitude(uint8_t newamplitude)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    amplitude = newamplitude;
    SREG = sreg_save;                   // restore interrupts
}

void DAC0_Timer3_SetOffset(int8_t newoffset)
{
    uint8_t sreg_save = SREG;           // Preserve Current Interrupt Status
    cli();
    offset = newoffset;
    SREG = sreg_save;                   // restore interrupts
}




//------------------------------------------------------------------------------------
// Keypad/button scanning
//------------------------------------------------------------------------------------
void ButtonMenuController(void)
{
    /*
        This is the place to implement functionality of key-based menu like one in the Timer1 
        interrupt example that demonstrates button polling with short, long and double-click 
        button detection. Such function can access and alter PWM setting for each channel.
        One button can be used to walk among the channels round robin style, while two other
        buttons can be used to increase or decrease the PWM parameter for the current channel.
    */

    // Note: we run this from within an interrupt - no need to call the "safe access" functions
    switch (keys_get() )
    {
        case B_K7:
            output_waveform = TRIANGLE;
            frequency   = 1;
            amplitude   = 0x3F;
            offset      = 0;
            break;

        case B_K6:
            output_waveform = SQUARE;
            frequency   = 1;
            amplitude   = 0xFF;
            offset      = 0;
            break;

        case B_K5:
            output_waveform = SINE;
            frequency   = 1;
            amplitude   = 0x7F;
            offset      = 0x7F;
            break;

        case B_K4:
            output_waveform = OFF;
            frequency   = 1;
            amplitude   = 0;
            offset      = 0;
            break;

        default:
            // if none or more than 1 key pressed
            break;
    }

}



//------------------------------------------------------------------------------------
// Timer1 Interrupt Functionality
//------------------------------------------------------------------------------------
void MyTimerFN (void)
{
    #ifdef DEBUG_205
        // Light LED.2 when the portion of the interrupt is serviced
        leds_set( leds_get() | 0x04 );
    #endif

    // do this at the beginning of the interrupt to minimize jitter caused by variable time needed to compute things
    PatternGenerator();                     // this must be called with the DAC sampling rate

    static uint16_t counter = 0;            // make sure that ( SAMPLING__FRQ / 100 ) fits the variable range!
    if (0<counter) counter--;
    else
    {
        counter = ( SAMPLING__FRQ / KEY_SCAN_FREQUENCY );

        ButtonMenuController();             // this must be called with the desired button scanning rate
    }

    #ifdef DEBUG_205
        // Light LED.2 when the portion of the interrupt is serviced
        leds_set( leds_get() & ~0x04 );
    #endif
}




int main(void)
{
    leds_init();
    leds_set(0);
    keys_init();

    Timer1_initialize( SAMPLING__FRQ , MyTimerFN, timer_prescale_1 );
    Timer2_PWM_initialize();    // change and observe PortB.7

    sei();
    while(1)
    {
    }
    
    return(0);
}
