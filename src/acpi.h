#ifndef ACPI_H
#define ACPI_H

#ifdef __cplusplus
extern "C"{
#endif

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

struct fadt {
    acpi_description_header header;
    uint32_t FIRMWARE_CTRL;
    void *DSDT;
    uint8_t INT_MODEL;
    uint8_t Preferred_PM_Profile;
    uint16_t SCI_INT;
    uint32_t SMI_CMD;
    uint8_t ACPI_ENABLE;
    uint8_t ACPI_DISABLE;
    uint8_t S4BIOS_REQ;
    uint8_t PSTATE_CNT;
    uint32_t PM1a_EVT_BLK;
    uint32_t PM1b_EVT_BLK;
    uint32_t PM1a_CNT_BLK;
    uint32_t PM1b_CNT_BLK;
    uint32_t PM2_CNT_BLK;
    uint32_t PM_TMR_BLK;
    uint32_t GPE0_BLK;
    uint32_t GPE1_BLK;
    uint8_t PM1_EVT_LEN;
    uint8_t PM1_CNT_LEN;
    uint8_t PM2_CNT_LEN;
    uint8_t PM_TMR_LEN;
    uint8_t GPE0_BLK_LEN;
    uint8_t GPE1_BLK_LEN;
    uint8_t GPE1_BASE;
    uint8_t CST_CNT;
    uint16_t P_LVL2_LAT;
    uint16_t P_LVL3_LAT;
    uint16_t FLUSH_SIZE;
    uint16_t FLUSH_STRIDE;
    uint8_t DUTY_OFFSET;
    uint8_t DUTY_WIDTH;
    uint8_t DAY_ALRM;
    uint8_t MON_ALRM;
    uint8_t CENTURY;
    uint16_t IAPC_BOOT_ARCH;
    uint8_t reserved1;
    uint32_t Flags;
    uint32_t RESET_REG[4];
    uint16_t ARM_BOOT_ARCH;
    uint8_t fadt_minor_version;
    uint64_t X_FIRMWARE_CTRL;
    uint64_t X_DSDT;
} PACKED;
typedef struct fadt fadt;

bool acpi_parse_tables(void);

extern rsdp *acpi_global_rsdp;
extern rsdt *acpi_global_rsdt;
extern fadt *acpi_global_fadt;
extern madt *acpi_global_madt;

#ifdef __cplusplus
}
#endif

#endif //ACPI_H
