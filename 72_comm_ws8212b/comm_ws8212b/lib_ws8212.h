//* Library to communicate with WS8212B - header file *
#ifndef lib_ws812b_h_
#define lib_ws812b_h_
#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_t;

/*
 * Note that PortC pin3 is hard-coded for the sake of transmission speed
 */

void ws8212_initialize();
void ws8212_program(const rgb_t data[], uint8_t length);
void ws8212_shutdown();

#endif
