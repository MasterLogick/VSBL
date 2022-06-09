#ifndef MEMORYBLOCK_H
#define MEMORYBLOCK_H

#include <Attributes.h>
#include <stdint.h>

typedef uint64_t base_t;
typedef uint64_t length_t;

struct MemoryBlock {
    base_t base;
    length_t length;

    inline base_t end() {
        return base + length - 1;
    }

} PACKED;


#endif //MEMORYBLOCK_H
