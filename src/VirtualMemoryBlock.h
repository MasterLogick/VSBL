#ifndef VIRTUALMEMORYBLOCK_H
#define VIRTUALMEMORYBLOCK_H

#include <stdint.h>
#include "MemoryBlock.h"

typedef uint32_t base_t;
typedef uint32_t length_t;

struct VirtualMemoryBlock {
    base_t base;
    length_t length;
    uint16_t flags;

    explicit VirtualMemoryBlock(MemoryBlock block) : base(block.base), length(block.length), flags(0) {}

    constexpr VirtualMemoryBlock(base_t base, length_t length) : base(base), length(length), flags(0) {}

    constexpr VirtualMemoryBlock(base_t base, length_t length, uint16_t flags) : base(base), length(length),
                                                                                 flags(flags) {}

    VirtualMemoryBlock() = default;

    inline base_t end() {
        return base + length - 1;
    }

    bool overlays(VirtualMemoryBlock &block) {
        return base <= block.base && block.end() <= end();
    }

    void split(VirtualMemoryBlock &block, VirtualMemoryBlock *high) {
        *high = VirtualMemoryBlock(block.end() + 1, length - block.length - (block.base - base), flags);
        length = block.base - base;
    }
} PACKED;

#endif //VIRTUALMEMORYBLOCK_H
