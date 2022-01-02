#include "acpi.h"
#include "string.h"

rsdp *acpi_find_rsdp(void) {
    char *ptr = (char *) 0x0e0000;
    for (; (uintptr_t) ptr < 0x0fffff; ptr++) {
        if (acpi_validate_rsdp((rsdp *) ptr)) {
            return (rsdp *) ptr;
        }
    }
    return (rsdp *) 0;
}

bool acpi_validate_rsdp(rsdp *ptr) {
    if (!memcmp(&ptr->signature, "RSD PTR ", 8))return false;
    char checksum = 0;
    char *chr_ptr = (char *) ptr;
    for (int i = 0; i < 20; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum != 0)return false;
    return true;
}

size_t acpi_get_tables_count(rsdt *ptr) {
    return (ptr->header.length - sizeof(acpi_description_header)) / sizeof(acpi_description_header *);
}

bool acpi_is_apic_header(acpi_description_header *header) {
    return memcmp(&header->signature, "APIC", 4);
}