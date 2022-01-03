#include "acpi.h"
#include "string.h"
#include "terminal.h"

rsdp *global_rsdp;
rsdt *global_rsdt;
fadt *global_fadt;
madt *global_madt;

rsdp *acpi_get_rsdp(void) {
    return global_rsdp;
}

rsdt *acpi_get_rsdt(void) {
    return global_rsdt;
}

fadt *acpi_get_fadt(void) {
    return global_fadt;
}

madt *acpi_get_madt(void) {
    return global_madt;
}

int acpi_get_tables_count(rsdt *ptr) {
    return (ptr->header.length - sizeof(acpi_description_header)) / sizeof(acpi_description_header *);
}

bool acpi_parse_fadt(fadt *ptr) {
    global_fadt = ptr;
    return true;
}

bool acpi_validate_rsdp(rsdp *ptr) {
    if (!memcmp(&ptr->signature, "RSD PTR ", 8))return false;
    uint8_t checksum = 0;
    uint8_t *chr_ptr = (uint8_t *) ptr;
    for (int i = 0; i < 20; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum != 0)return false;
    return true;
}

bool acpi_validate_table_header(acpi_description_header *header) {
    uint8_t checksum = 0;
    for (uint8_t *i = (uint8_t *) header; (uint32_t) (i - (uint8_t *) header) + 1 <= header->length; ++i) {
        checksum += *i;
    }
    return !checksum;
}

bool acpi_parse_madt(madt *ptr) {
    global_madt = ptr;
    ics_header *ics = &ptr->ics[0];
    for (int j = 0; (uint32_t) ((char *) ics - (char *) ptr) < ptr->header.length; ++j) {
        switch (ics->type) {
            case 0:
                terminal_printf("ACPI: MADT: ics table %d: structure type: processor local APIC\n", j);
                ics_processor_local_apic *apic = (ics_processor_local_apic *) ics;
                terminal_printf("ACPI: MADT: ics table %d processor UID %d -> APIC %d\n",
                                j, apic->processor_uid, apic->apic_id);
                break;
            case 1:
                terminal_printf("ACPI: MADT: ics table %d: structure type: IO APIC\n", j);
                ics_io_apic *io_apic = (ics_io_apic *) ics;
                terminal_printf("ACPI: MADT: ics table %d: IO APIC id: %d\n", j, io_apic->io_apic_id);
                terminal_printf("ACPI: MADT: ics table %d: IO APIC address: %!x\n", j, io_apic->io_apic);
                terminal_printf("ACPI: MADT: ics table %d: IO APIC global system interrupt base: %!x\n", j,
                                io_apic->global_system_interrupt_base);
                global_io_apic_base = io_apic->io_apic;
                break;
            case 2:
                terminal_printf("ACPI: MADT: ics table %d: structure type: interrupt source override\n", j);
                ics_iso *iso = (ics_iso *) ics;
                terminal_printf("ACPI: MADT: ics table %d: interrupt source override: bus %d source %d:\n",
                                j, iso->bus, iso->source);
                terminal_printf("ACPI: MADT: ics table %d: interrupt source override: global system interrupt: %d\n",
                                j, iso->global_system_interrupt);
                break;
            default:
                terminal_printf("ACPI: MADT: ics table %d: structure type: unknown(%d)\n", j, *ics);
                break;
        }
        ics = (ics_header *) (((char *) ics) + ics->length);
    }
    return true;
}

rsdp *acpi_find_rsdp(void) {
    char *ptr = (char *) 0x0e0000;
    for (; (uintptr_t) ptr < 0x0fffff; ptr++) {
        if (acpi_validate_rsdp((rsdp *) ptr)) {
            return (rsdp *) ptr;
        }
    }
    return (rsdp *) 0;
}

bool acpi_parse_tables(void) {
    global_rsdp = acpi_find_rsdp();
    if (!global_rsdp) {
        terminal_printf("ACPI: RSDP: not found\n");
        return false;
    }
    if (!acpi_validate_rsdp(global_rsdp)) {
        terminal_printf("ACPI: RSDP: validation failed\n");
        return false;
    }
    global_rsdt = global_rsdp->rsdt;
    if (!global_rsdt) {
        terminal_printf("ACPI: RSDT: not found\n");
        return false;
    }
    if (!acpi_validate_table_header(&global_rsdt->header)) {
        terminal_printf("ACPI: RSDT: validation failed\n");
        return false;
    }
    int tables_count = acpi_get_tables_count(global_rsdt);
    terminal_printf("ACPI: RSDT: table contains %d entries\n", tables_count);
    for (int i = 0; i < tables_count; ++i) {
        acpi_description_header *header = global_rsdt->tables[i];
        if (!acpi_validate_table_header(header)) {
            terminal_printf("ACPI: table %d: validation failed\n", i);
            continue;
        }
        terminal_printf("ACPI: table %d: signature: %[4]s\n", i, &header->signature);
        if (memcmp(&header->signature, "FADT", 4)) {
            acpi_parse_fadt((fadt *) header);
        } else if (memcmp(&header->signature, "APIC", 4)) {
            acpi_parse_madt((madt *) header);
        } else {
            terminal_printf("ACPI: table %d: parser not found\n", i);
        }
    }
    return true;
}

bool acpi_is_apic_header(acpi_description_header *header) {
    return memcmp(&header->signature, "APIC", 4);
}