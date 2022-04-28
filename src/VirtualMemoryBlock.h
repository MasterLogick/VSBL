#ifndef VIRTUALMEMORYBLOCK_H
#define VIRTUALMEMORYBLOCK_H

#include <stdint.h>
#include "MemoryBlock.h"

struct VirtualMemoryBlock : public MemoryBlock {
    explicit VirtualMemoryBlock(MemoryBlock block) : MemoryBlock(block) {}

    constexpr VirtualMemoryBlock(base_t base, length_t length) : MemoryBlock({base, length}) {}

    VirtualMemoryBlock() = default;

    bool overlays(VirtualMemoryBlock &block) {
        return base <= block.base && block.end() <= end();
    }

    void split(VirtualMemoryBlock &block, VirtualMemoryBlock *high) {
        *high = VirtualMemoryBlock(block.end() + 1, length - block.length - (block.base - base));
        length = block.base - base;
    }
} PACKED;

#endif //VIRTUALMEMORYBLOCK_H
