#ifndef MSR_C
#define MSR_C

#include <stdint.h>

void _msr_set_asm(uint32_t reg, uint64_t value);

uint64_t _msr_get_asm(uint32_t reg);

#endif //MSR_C