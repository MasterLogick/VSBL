#ifndef UTIL_H
#define UTIL_H

#include "Attributes.h"

extern "C" {
void _int3_asm();

uint32_t _get_time_asm();

NORETURN void _hlt_loop();
};
#endif //UTIL_H