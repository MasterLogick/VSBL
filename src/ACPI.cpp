#include "ACPI.h"
#include "string.h"
#include "terminal.h"

RSDP *GlobalRSDP;
RSDT *GlobalRSDT;
FADT *GlobalFADT;
MADT *GlobalMADT;

bool DescriptionHeader::validate() {
    uint8_t checksum = 0;
    for (uint8_t *i = (uint8_t *) this; (uint32_t) (i - (uint8_t *) this) + 1 <= this->length; ++i) {
        checksum += *i;
    }
    return !checksum;
}

int RSDT::getTablesCount() {
    return (this->length - sizeof(DescriptionHeader)) / sizeof(void *);
}

void FADT::parse() {
    GlobalFADT = this;
}

bool RSDP::validate() {
    if (!memcmp(&this->signature, "RSD PTR ", 8))return false;
    uint8_t checksum = 0;
    uint8_t *chr_ptr = (uint8_t *) this;
    for (int i = 0; i < 20; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum != 0)return false;
    return true;
}

void MADT::parse() {
    GlobalMADT = this;
    ICSHeader *header = &this->ics[0];
    for (int j = 0; (uint32_t) ((char *) header - (char *) this) < this->length; ++j) {
        switch (header->type) {
            case 0: {
                terminal_printf("ACPI: MADT: ics table %d: structure type: processor local APIC\n", j);
                ProcessorLocalApic *apic = (ProcessorLocalApic *) header;
                terminal_printf("ACPI: MADT: ics table %d: processor UID %d -> APIC %d\n",
                                j, apic->acpiProcessorID, apic->apicID);
                break;
            }
            case 1: {
                terminal_printf("ACPI: MADT: ics table %d: structure type: IO APIC\n", j);
                AcpiIOApic *io_apic = (AcpiIOApic *) header;
                terminal_printf("ACPI: MADT: ics table %d: IO APIC id: %d\n", j, io_apic->ioApicID);
                terminal_printf("ACPI: MADT: ics table %d: IO APIC address: %!x\n", j, io_apic->ioApic);
                terminal_printf("ACPI: MADT: ics table %d: IO APIC global system interrupt base: %!x\n", j,
                                io_apic->globalSystemInterruptBase);
                GlobalIOAPIC = io_apic->ioApic;
                break;
            }
            case 2: {
                terminal_printf("ACPI: MADT: ics table %d: structure type: interrupt source override\n", j);
                ISO *iso = (ISO *) header;
                terminal_printf("ACPI: MADT: ics table %d: interrupt source override: bus %d source %d:\n",
                                j, iso->bus, iso->source);
                terminal_printf(
                        "ACPI: MADT: ics table %d: interrupt source override: global system interrupt: %d\n",
                        j, iso->globalSystemInterrupt);
                break;
            }
            default: {
                terminal_printf("ACPI: MADT: ics table %d: structure type: unknown(%d)\n", j, *header);
                break;
            }
        }
        header = (ICSHeader *) (((char *) header) + header->length);
    }
}

RSDP *findRSDP() {
    char *ptr = (char *) 0x0e0000;
    for (; (uintptr_t) ptr < 0x0fffff; ptr++) {
        if (((RSDP *) ptr)->validate()) {
            return (RSDP *) ptr;
        }
    }
    return (RSDP *) 0;
}

bool parseACPITables() {
    GlobalRSDP = findRSDP();
    if (!GlobalRSDP) {
        terminal_printf("ACPI: RSDP: not found\n");
        return false;
    }
    if (!GlobalRSDP->validate()) {
        terminal_printf("ACPI: RSDP: validation failed\n");
        return false;
    }
    GlobalRSDT = GlobalRSDP->rsdt;
    if (!GlobalRSDT) {
        terminal_printf("ACPI: RSDT: not found\n");
        return false;
    }
    if (!GlobalRSDT->validate()) {
        terminal_printf("ACPI: RSDT: validation failed\n");
        return false;
    }
    int tablesCount = GlobalRSDT->getTablesCount();
    terminal_printf("ACPI: RSDT: table contains %d entries\n", tablesCount);
    for (int i = 0; i < tablesCount; ++i) {
        DescriptionHeader *header = GlobalRSDT->tables[i];
        if (!header->validate()) {
            terminal_printf("ACPI: table %d: validation failed\n", i);
            continue;
        }
        terminal_printf("ACPI: table %d: signature: %[4]s\n", i, &header->signature);
        if (memcmp(&header->signature, "FADT", 4)) {
            ((FADT *) header)->parse();
        } else if (memcmp(&header->signature, "APIC", 4)) {
            ((MADT *) header)->parse();
        } else {
            terminal_printf("ACPI: table %d: parser not found\n", i);
        }
    }
    return true;
}