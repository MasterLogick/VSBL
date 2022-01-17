#ifndef DEFINES_H
#define DEFINES_H

#include <limits.h>
#include <stdint.h>

#undef VSBL_KERNEL_64
#undef VSBL_KERNEL_32

#if (__WORDSIZE == 64)
#define VSBL_KERNEL_64
typedef uint64_t address_t;
#error This kernel is 32 bit only
#else
#define VSBL_KERNEL_32
typedef uint32_t address_t;
#endif

#endif //DEFINES_H
