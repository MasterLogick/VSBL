#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include "Attributes.h"
#include <stdint.h>

typedef uint64_t base_long_t;
typedef uint64_t length_long_t;

struct MemoryBlock {
    base_long_t base;
    length_long_t length;

    inline base_long_t end() {
        return base + length - 1;
    }

} PACKED;


#endif //MEMORYBLOCK_H
