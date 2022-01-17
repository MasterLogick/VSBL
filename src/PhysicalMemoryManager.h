#ifndef PHYSICALMEMORYMANAGER_H
#define PHYSICALMEMORYMANAGER_H

#include <stdint.h>
#include <stddef.h>
#include "Attributes.h"
#include "MemoryBlock.h"
#include "PhysicalMemoryBlock.h"

#define PMM_MAX_ENTRY_COUNT 30


struct PhysicalMemoryManager {
    PhysicalMemoryBlock conventionalMemory[PMM_MAX_ENTRY_COUNT];
    int conventionalBlocksCount;
    PhysicalMemoryBlock unusableMemory[PMM_MAX_ENTRY_COUNT];
    int unusableBlocksCount;

    void addConventionalBlock(PhysicalMemoryBlock &block);

    void extractConventionalBlocks();
};

extern PhysicalMemoryManager GlobalPMM;

#endif //PHYSICALMEMORYMANAGER_H
