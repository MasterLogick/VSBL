#include "ACPI.h"
#include "../string.h"
#include "../iostream.h"

RSDP *GlobalRSDP;
XSDT *GlobalXSDT;
FADT *GlobalFADT;
MADT *GlobalMADT;

bool DescriptionHeader::isValid() {
    uint8_t checksum = 0;
    for (uint8_t *i = (uint8_t *) this; (uint32_t) (i - (uint8_t *) this) + 1 <= length; ++i) {
        checksum += *i;
    }
    return !checksum;
}

int XSDT::getTablesCount() {
    return (length - sizeof(DescriptionHeader)) / sizeof(void *);
}

void FADT::parse() {
}

bool RSDP::isValid() {
    if (!memcmp(&this->signature, "RSD PTR ", 8))return false;
    uint8_t checksum = 0;
    uint8_t *chr_ptr = (uint8_t *) this;
    for (int i = 0; i < 20; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum)return false;
    if (!revision)return false;
    for (unsigned int i = 20; i < length; ++i) {
        checksum += chr_ptr[i];
    }
    if (checksum)return false;
    return true;
}

void MADT::parse() {
    ICSHeader *header = &ics[0];
    for (int j = 0; (uint32_t) ((char *) header - (char *) this) < length; ++j) {
        switch (header->type) {
            case 0: {
                cout << "ACPI: MADT: ics table " << j << ": structure type: processor local APIC\n";
                ProcessorLocalApic *apic = (ProcessorLocalApic *) header;
                cout << "ACPI: MADT: ics table " << j << ": processor UID " << apic->acpiProcessorID << " -> APIC "
                     << apic->apicID << "\n";
                break;
            }
            case 1: {
                cout << "ACPI: MADT: ics table " << j << ": structure type: IO APIC\n";
                AcpiIOApic *io_apic = (AcpiIOApic *) header;
                cout << "ACPI: MADT: ics table " << j << ": IO APIC id: " << io_apic->ioApicID
                     << "\n"
                     << "ACPI: MADT: ics table " << j << ": IO APIC address: " << cout.full
                     << cout.HEX << io_apic->ioApic << cout.defaults << "\n"
                     << "ACPI: MADT: ics table " << j << ": IO APIC global system interrupt base: " << cout.HEX
                     << cout.full << io_apic->globalSystemInterruptBase << cout.defaults << "\n";
                GlobalIOAPIC = io_apic->ioApic;
                break;
            }
            case 2: {
                cout << "ACPI: MADT: ics table " << j << ": structure type: interrupt source override\n";
                ISO *iso = (ISO *) header;
                cout << "ACPI: MADT: ics table " << j << ": interrupt source override: bus " << iso->bus << " source "
                     << iso->source << "\n"
                     << "ACPI: MADT: ics table " << j << ": interrupt source override: global system interrupt: "
                     << iso->globalSystemInterrupt << "\n";
                break;
            }
            default: {
                cout << "ACPI: MADT: ics table " << j << ": structure type: unknown(" << header->type << ")\n";
                break;
            }
        }
        header = (ICSHeader *) (((uint8_t *) header) + header->length);
    }
}

RSDP *findRSDP() {
    uint8_t *volatile ptr = reinterpret_cast<uint8_t *>(0x0e0000);
    for (; (uintptr_t) ptr < 0x0fffff; ptr++) {
        if (((RSDP *) ptr)->isValid()) {
            return (RSDP *) ptr;
        }
    }
    return nullptr;
}

bool parseACPITables() {
    GlobalRSDP = findRSDP();
    if (!GlobalRSDP) {
        cout << "ACPI: RSDP: not found\n";
        return false;
    }
    if (!GlobalRSDP->isValid()) {
        cout << "ACPI: RSDP: validation failed\n";
        return false;
    }
    GlobalXSDT = GlobalRSDP->xsdt;
    if (!GlobalXSDT) {
        cout << "ACPI: XSDT: not found\n";
        return false;
    }
    if (!GlobalXSDT->isValid()) {
        cout << "ACPI: XSDT: validation failed\n";
        return false;
    }
    int tablesCount = GlobalXSDT->getTablesCount();
    cout << "ACPI: XSDT: table contains " << tablesCount << " entries\n";
    for (int i = 0; i < tablesCount; ++i) {
        DescriptionHeader *header = GlobalXSDT->tables[i];
        if (!header->isValid()) {
            cout << "ACPI: table " << i << ": validation failed\n";
            continue;
        }
        cout << "ACPI: table " << i << ": signature: " << header->signature[0] << header->signature[1]
             << header->signature[2] << header->signature[3] << "\n";
        if (memcmp(&header->signature, "FADT", 4)) {
            GlobalFADT = ((FADT *) header);
            GlobalFADT->parse();
        } else if (memcmp(&header->signature, "APIC", 4)) {
            GlobalMADT = ((MADT *) header);
            GlobalMADT->parse();
        } else {
            cout << "ACPI: table " << i << ": parser not found\n";
        }
    }
    return true;
}