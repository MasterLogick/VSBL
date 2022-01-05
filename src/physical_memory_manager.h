#ifndef PHYSICAL_MEMORY_MANAGER_H
#define PHYSICAL_MEMORY_MANAGER_H

#ifdef __cplusplus
extern "C"{
#endif

#include "attributes.h"
#include <stdint.h>
#include <stddef.h>

#define PMM_MAX_ENTRY_COUNT 30

struct pmm_entry {
    uint64_t base_addr;
    uint64_t length;
    uint32_t type;
    uint32_t acpi;
} PACKED;
typedef struct pmm_entry pmm_entry;

extern pmm_entry pmm_conventional_blocks[PMM_MAX_ENTRY_COUNT];
extern int pmm_conventional_blocks_count;

void pmm_extract_usable_blocks(void);

#ifdef __cplusplus
}
#endif

#endif //PHYSICAL_MEMORY_MANAGER_H
