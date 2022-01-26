#include "PhysicalMemoryManager.h"
#include "iostream.h"


const PhysicalMemoryBlock pmm_predefined_unusable_blocks[] = {
        {0,       0x600,   2, 1},
        {0x80000, 0x80000, 2, 1}
};

PhysicalMemoryManager GlobalPMM;

extern struct MemoryMap {
    PhysicalMemoryBlock blocks[PMM_MAX_ENTRY_COUNT];
    int blocksCount;
} _PAM_asm;

void PhysicalMemoryBlock::removeIntersections(PhysicalMemoryBlock *blocks, int count) {
    base_t start = base;
    base_t end = this->end();
    for (int i = 0; i < count; ++i) {
        base_t unusable_start = blocks[i].base;
        base_t unusable_end = blocks[i].end();
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
    PhysicalMemoryBlock potentialMemory[PMM_MAX_ENTRY_COUNT];
    int potentialBlockCount = 0;
    unusableBlocksCount = 0;
    for (int i = 0; i < _PAM_asm.blocksCount; ++i) {
        if (_PAM_asm.blocks[i].length == 0) {
            continue;
        }
        if (_PAM_asm.blocks[i].type == 1 && (_PAM_asm.blocks[i].acpi & 1)) {
            potentialMemory[potentialBlockCount] = _PAM_asm.blocks[i];
            potentialBlockCount++;
        } else {
            unusableMemory[unusableBlocksCount] = _PAM_asm.blocks[i];
            unusableBlocksCount++;
        }
    }
    cout << "PMM: potential conventional blocks count: " << potentialBlockCount << "\n";
    cout << "PMM: unusable blocks count: "
         << unusableBlocksCount + sizeof(pmm_predefined_unusable_blocks) / sizeof(pmm_predefined_unusable_blocks[0])
         << "\n";
    conventionalBlocksCount = 0;
    for (int i = 0; i < potentialBlockCount; ++i) {
        addConventionalBlock(potentialMemory[i]);
    }
    cout << "PMM: memory map ready\nPMM: total conventional blocks count: " << conventionalBlocksCount << "\n";
}
