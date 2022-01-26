#include "MADT.h"
#include "ics/ProcessorLocalApic.h"
#include "ics/AcpiIOApic.h"
#include "ics/ISO.h"
#include "../../iostream.h"
#include "../../IOAPIC.h"

MADT *GlobalMADT;

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