#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "attributes.h"
#include "apic.h"
#include "ioapic.h"

struct acpi_description_header {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char oem_id[6];
    char oem_table_id[8];
    uint32_t oem_revision;
    uint32_t creator_id;
    uint32_t creator_revision;
} PACKED;
typedef struct acpi_description_header acpi_description_header;

struct rsdt {
    acpi_description_header header;
    acpi_description_header *tables[1];
} PACKED;
typedef struct rsdt rsdt;

struct processor_local_apic {
    uint8_t type;
    uint8_t length;
    uint8_t acpi_processor_id;
    uint8_t apic_id;
    uint32_t flags;
} PACKED;
typedef struct processor_local_apic processor_local_apic;

struct acpi_io_apic {
    uint8_t type;
    uint8_t length;
    uint8_t io_apic_id;
    uint8_t reserved;
    void *io_apic;
    uint32_t system_vector_base;
} PACKED;
typedef struct acpi_io_apic acpi_io_apic;

struct ics_header {
    uint8_t type;
    uint8_t length;
} PACKED;
typedef struct ics_header ics_header;

struct madt {
    acpi_description_header header;
    apic_base *apic_table;
    uint32_t flags;
    ics_header ics[0];
} PACKED;

typedef struct madt madt;
struct rsdp {
    char signature[8];
    uint8_t checksum;
    char oem_id[6];
    uint8_t revision;
    rsdt *rsdt;
} PACKED;

typedef struct rsdp rsdp;

struct ics_processor_local_apic {
    ics_header header;
    uint8_t processor_uid;
    uint8_t apic_id;
    uint32_t flags;
} PACKED;
typedef struct ics_processor_local_apic ics_processor_local_apic;

struct ics_io_apic {
    ics_header header;
    uint8_t io_apic_id;
    uint8_t reserved;
    io_apic_base io_apic;
    uint32_t global_system_interrupt_base;
} PACKED;
typedef struct ics_io_apic ics_io_apic;

struct ics_iso {
    ics_header header;
    uint8_t bus;
    uint8_t source;
    uint32_t global_system_interrupt;
    uint16_t flags;
} PACKED;
typedef struct ics_iso ics_iso;

rsdp *acpi_find_rsdp(void);

bool acpi_validate_rsdp(rsdp *ptr);

size_t acpi_get_tables_count(rsdt *ptr);

bool acpi_is_apic_header(acpi_description_header *header);

#endif //ACPI_H
