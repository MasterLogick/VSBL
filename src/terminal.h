#ifndef TERMINAL_C
#define TERMINAL_C

#include <stdint.h>

void terminal_writestring(const char* data);
void terminal_putchar(char c);
void terminal_initialize(void);
void terminal_print_int(uint32_t val);

#endif //TERMINAL_C