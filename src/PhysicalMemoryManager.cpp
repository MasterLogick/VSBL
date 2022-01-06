#include "PhysicalMemoryManager.h"
#include "terminal.h"


MemoryLayout GlobalMemoryLayout;
MemoryBlock pmm_predefined_unusable_blocks[2] = {
        {0,       0x500,   2, 1},
        {0x80000, 0x80000, 2, 1}
};

void MemoryBlock::removeIntersections(MemoryBlock *blocks, int count) {
    uint64_t start = address;
    uint64_t end = address + length - 1;
    for (int i = 0; i < count; ++i) {
        uint64_t unusable_start = blocks[i].address;
        uint64_t unusable_end = blocks[i].address + blocks[i].length - 1;
        if (unusable_end >= start && unusable_start <= start) {
            if (unusable_end != UINT64_MAX) {
                start = unusable_end + 1;
            } else {
                start = UINT64_MAX;
            }
        }
        if (unusable_start <= end && unusable_end >= end) {
            if (unusable_start != 0) {
                end = unusable_start - 1;
            } else {
                end = 0;
            }
        }
        if (start >= end) {
            address = 0;
            length = 0;
            type = 0;
            acpi = 0;
            return;
        }
        if (unusable_start > start && unusable_end < end) {
            MemoryBlock low = {start, end - start + 1, 1, acpi};
            GlobalMemoryLayout.addConventionalBlock(low);
            if (unusable_end != UINT64_MAX) {
                start = unusable_end + 1;
            } else {
                start = UINT64_MAX;
            }
        }
        if (start >= end) {
            address = 0;
            length = 0;
            type = 0;
            acpi = 0;
            return;
        }
    }
    address = start;
    length = end - start + 1;
}

void MemoryLayout::addConventionalBlock(MemoryBlock block) {
    block.removeIntersections(unusableMemory, unusableBlocksCount);
    if (block.length == 0) {
        return;
    }
    block.removeIntersections(pmm_predefined_unusable_blocks,
                              sizeof(pmm_predefined_unusable_blocks) / sizeof(pmm_predefined_unusable_blocks[0]));
    if (block.length == 0) {
        return;
    }

    block.removeIntersections(conventionalMemory, conventionalBlocksCount);
    if (block.length == 0) {
        return;
    }
    if (conventionalBlocksCount < PMM_MAX_ENTRY_COUNT) {
        conventionalMemory[conventionalBlocksCount] = block;
        conventionalBlocksCount++;
    }
}

void MemoryLayout::extractConventionalBlocks() {
    MemoryBlock potentionalMemory[PMM_MAX_ENTRY_COUNT];
    int potentialBlockCount = 0;
    unusableBlocksCount = 0;
    for (int i = 0; i < conventionalBlocksCount; ++i) {
        if (conventionalMemory[i].length == 0) {
            continue;
        }
        if (conventionalMemory[i].type == 1 && (conventionalMemory[i].acpi & 1)) {
            potentionalMemory[potentialBlockCount] = conventionalMemory[i];
            potentialBlockCount++;
        } else {
            unusableMemory[unusableBlocksCount] = conventionalMemory[i];
            unusableBlocksCount++;
        }
    }
    terminal_printf("PMM: potential conventional blocks count: %d\n", potentialBlockCount);
    terminal_printf("PMM: unusable blocks count: %d\n", unusableBlocksCount +
                                                        sizeof(pmm_predefined_unusable_blocks) /
                                                        sizeof(pmm_predefined_unusable_blocks[0]));
    conventionalBlocksCount = 0;
    for (int i = 0; i < potentialBlockCount; ++i) {
        addConventionalBlock(potentionalMemory[i]);
    }
    terminal_printf("PMM: memory map ready\n");
    terminal_printf("PMM: total conventional blocks count: %d\n", conventionalBlocksCount);
}
