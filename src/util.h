#ifndef UTIL_H
#define UTIL_H

#include <Attributes.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void _int3_asm();

extern uint32_t _get_time_asm();

NORETURN extern void _hlt_loop();

#ifdef __cplusplus
}
#endif

#endif //UTIL_H