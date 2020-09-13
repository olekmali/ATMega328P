//* stdio_helper.c makes printf and scanf work *
#ifndef STDIO_HELPER_H_
#define STDIO_HELPER_H_
#include <stdio.h>

void std_streams_initialize();

extern FILE* com1;
// ^^^ extern promises that the actual variable is declared somewhere and can be found later by the linker
// this IS NOT the actual variable declaration



#endif /* STDIO_HELPER_H_ */
