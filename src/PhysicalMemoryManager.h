#ifndef PHYSICALMEMORYMANAGER_H
#define PHYSICALMEMORYMANAGER_H

#include "Attributes.h"
#include <stdint.h>
#include <stddef.h>

#define PMM_MAX_ENTRY_COUNT 30

struct MemoryBlock {
    uint64_t address;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;

    void removeIntersections(MemoryBlock *blocks, int count);
} PACKED;

struct MemoryLayout {
    MemoryBlock conventionalMemory[PMM_MAX_ENTRY_COUNT];
    int conventionalBlocksCount;
    MemoryBlock unusableMemory[PMM_MAX_ENTRY_COUNT];
    int unusableBlocksCount;

    void addConventionalBlock(MemoryBlock block);

    void extractConventionalBlocks();
} PACKED;

extern MemoryLayout GlobalMemoryLayout;
#endif //PHYSICALMEMORYMANAGER_H
