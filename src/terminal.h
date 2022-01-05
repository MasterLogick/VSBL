#ifndef TERMINAL_C
#define TERMINAL_C

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

void terminal_initialize(void);

void terminal_printf(const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif //TERMINAL_C