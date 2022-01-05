#include "physical_memory_manager.h"
#include "terminal.h"

pmm_entry pmm_conventional_blocks[PMM_MAX_ENTRY_COUNT];
int pmm_conventional_blocks_count;
pmm_entry pmm_unusable_blocks[PMM_MAX_ENTRY_COUNT];
int pmm_unusable_blocks_count;
pmm_entry pmm_predefined_unusable_blocks[2] = {
        {0,       0x500,   2, 1},
        {0x80000, 0x80000, 2, 1}
};

void pmm_add_conventional_block(pmm_entry block);

pmm_entry pmm_remove_intersections(pmm_entry block, pmm_entry *memory, int entity_count) {
    uint64_t start = block.base_addr;
    uint64_t end = block.base_addr + block.length - 1;
    for (int i = 0; i < entity_count; ++i) {
        uint64_t unusable_start = memory[i].base_addr;
        uint64_t unusable_end = memory[i].base_addr + memory[i].length - 1;
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
            pmm_entry val = {0, 0, 0, 0};
            return val;
        }
        if (unusable_start > start && unusable_end < end) {
            pmm_entry low = {start, end - start + 1, 1, block.acpi};
            pmm_add_conventional_block(low);
            if (unusable_end != UINT64_MAX) {
                start = unusable_end + 1;
            } else {
                start = UINT64_MAX;
            }
        }
        if (start >= end) {
            pmm_entry val = {0, 0, 0, 0};
            return val;
        }
    }
    pmm_entry val = {start, end - start + 1, 1, block.acpi};
    return val;
}

void pmm_add_conventional_block(pmm_entry block) {
    block = pmm_remove_intersections(block, pmm_unusable_blocks, pmm_unusable_blocks_count);
    if (block.length == 0) {
        return;
    }
    block = pmm_remove_intersections(block, pmm_predefined_unusable_blocks,
                                     sizeof(pmm_predefined_unusable_blocks) /
                                     sizeof(pmm_predefined_unusable_blocks[0]));
    if (block.length == 0) {
        return;
    }
    block = pmm_remove_intersections(block, pmm_conventional_blocks, pmm_conventional_blocks_count);
    if (block.length == 0) {
        return;
    }
    if (pmm_conventional_blocks_count < PMM_MAX_ENTRY_COUNT) {
        pmm_conventional_blocks[pmm_conventional_blocks_count] = block;
        pmm_conventional_blocks_count++;
    }
}

void pmm_extract_usable_blocks(void) {
    pmm_entry conventional_memory[PMM_MAX_ENTRY_COUNT];
    int conventional_count = 0;
    pmm_unusable_blocks_count = 0;
    for (int i = 0; i < PMM_MAX_ENTRY_COUNT; ++i) {
        if (pmm_conventional_blocks[i].length == 0) {
            continue;
        }
        if (pmm_conventional_blocks[i].type == 1 && (pmm_conventional_blocks[i].acpi & 1)) {
            conventional_memory[conventional_count] = pmm_conventional_blocks[i];
            conventional_count++;
        } else {
            pmm_unusable_blocks[pmm_unusable_blocks_count] = pmm_conventional_blocks[i];
            pmm_unusable_blocks_count++;
        }
    }
    terminal_printf("PMM: potential conventional blocks count: %d\n", conventional_count);
    terminal_printf("PMM: unusable blocks count: %d\n", pmm_unusable_blocks_count +
                                                        sizeof(pmm_predefined_unusable_blocks) /
                                                        sizeof(pmm_predefined_unusable_blocks[0]));
    pmm_conventional_blocks_count = 0;
    for (int i = 0; i < conventional_count; ++i) {
        pmm_add_conventional_block(conventional_memory[i]);
    }
    terminal_printf("PMM: memory map ready\n");
    terminal_printf("PMM: total conventional blocks count: %d\n", pmm_conventional_blocks_count);
}