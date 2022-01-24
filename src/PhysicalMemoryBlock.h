#ifndef PHYSICALMEMORYBLOCK_H
#define PHYSICALMEMORYBLOCK_H

#include <stdint.h>
#include "MemoryBlock.h"

struct PhysicalMemoryBlock : public MemoryBlock {
    uint32_t type;
    uint32_t acpi;

    void removeIntersections(PhysicalMemoryBlock *blocks, int count);

    constexpr inline PhysicalMemoryBlock(base_t base, length_t length, uint32_t type, uint32_t acpi) :
            MemoryBlock({base, length}), type(type), acpi(acpi) {}

    PhysicalMemoryBlock() = default;
} PACKED;

#endif //PHYSICALMEMORYBLOCK_H
