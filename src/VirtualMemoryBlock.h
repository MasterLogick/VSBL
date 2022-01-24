#ifndef VIRTUALMEMORYBLOCK_H
#define VIRTUALMEMORYBLOCK_H

#include <stdint.h>
#include "MemoryBlock.h"

struct VirtualMemoryBlock : public MemoryBlock {
    uint16_t flags;

    explicit VirtualMemoryBlock(MemoryBlock block) : MemoryBlock(block), flags(0) {}

    constexpr VirtualMemoryBlock(base_t base, length_t length) : MemoryBlock({base, length}), flags(0) {}

    constexpr VirtualMemoryBlock(base_t base, length_t length, uint16_t flags) : MemoryBlock({base, length}),
                                                                                 flags(flags) {}

    VirtualMemoryBlock() = default;

    bool overlays(VirtualMemoryBlock &block) {
        return base <= block.base && block.end() <= end();
    }

    void split(VirtualMemoryBlock &block, VirtualMemoryBlock *high) {
        *high = VirtualMemoryBlock(block.end() + 1, length - block.length - (block.base - base), flags);
        length = block.base - base;
    }
} PACKED;

#endif //VIRTUALMEMORYBLOCK_H
