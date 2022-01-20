#include "PhysicalMemoryManager.h"
#include "terminal.h"


const PhysicalMemoryBlock pmm_predefined_unusable_blocks[3] = {
        {0,           0x600,              2, 1},
        {0x80000,     0x80000,            2, 1},
        {0x100000000, 0xFFFFFFFF00000000, 2, 1}
};

PhysicalMemoryManager GlobalPMM;

extern struct MemoryMap {
    PhysicalMemoryBlock blocks[PMM_MAX_ENTRY_COUNT];
    int blocksCount;
} _PAM_asm;

void PhysicalMemoryBlock::removeIntersections(PhysicalMemoryBlock *blocks, int count) {
    uint64_t start = base;
    uint64_t end = this->end();
    for (int i = 0; i < count; ++i) {
        uint64_t unusable_start = blocks[i].base;
        uint64_t unusable_end = blocks[i].end();
        if (blocks[i].length == 0)
            terminal_printf("%d %!x%!x %!x%!x\n    %!x%!x %!x%!x\n", i, start, end, unusable_start, unusable_end);
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
            base = 0;
            length = 0;
            type = 0;
            acpi = 0;
            return;
        }
        if (unusable_start > start && unusable_end < end) {
            PhysicalMemoryBlock low{start, end - start + 1, 1, acpi};
            GlobalPMM.addConventionalBlock(low);
            if (unusable_end != UINT64_MAX) {
                start = unusable_end + 1;
            } else {
                start = UINT64_MAX;
            }
        }
        if (start >= end) {
            base = 0;
            length = 0;
            type = 0;
            acpi = 0;
            return;
        }
    }
    base = start;
    length = end - start + 1;
}

void PhysicalMemoryManager::addConventionalBlock(PhysicalMemoryBlock &block) {
    block.removeIntersections(unusableMemory, unusableBlocksCount);
    if (block.length == 0) {
        return;
    }
    block.removeIntersections(const_cast<PhysicalMemoryBlock *>(pmm_predefined_unusable_blocks),
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

void PhysicalMemoryManager::extractConventionalBlocks() {
    PhysicalMemoryBlock potentionalMemory[PMM_MAX_ENTRY_COUNT];
    int potentialBlockCount = 0;
    unusableBlocksCount = 0;
    for (int i = 0; i < _PAM_asm.blocksCount; ++i) {
        if (_PAM_asm.blocks[i].length == 0) {
            continue;
        }
        if (_PAM_asm.blocks[i].type == 1 && (_PAM_asm.blocks[i].acpi & 1)) {
            potentionalMemory[potentialBlockCount] = _PAM_asm.blocks[i];
            potentialBlockCount++;
        } else {
            unusableMemory[unusableBlocksCount] = _PAM_asm.blocks[i];
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
