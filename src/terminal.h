#ifndef TERMINAL_C
#define TERMINAL_C

#include <stdint.h>

void terminal_initialize();

void terminal_printf(const char *format, ...);

void terminal_writestring(const char *data);

void terminal_putchar(char c);

void terminal_print_int64(uint64_t val, int radix, bool full);

#endif //TERMINAL_C