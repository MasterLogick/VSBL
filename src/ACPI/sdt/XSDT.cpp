#include "XSDT.h"
#include "../../iostream.h"
#include "FADT.h"
#include "MADT.h"

XSDT *GlobalXSDT;

int XSDT::getTablesCount() {
    return (length - sizeof(DescriptionHeader)) / sizeof(void *);
}

void XSDT::parseTables() {
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
        if (memcmp(&header->signature, "FACP", 4)) {
            GlobalFADT = ((FADT *) header);
            GlobalFADT->parse();
        } else if (memcmp(&header->signature, "APIC", 4)) {
            GlobalMADT = ((MADT *) header);
            GlobalMADT->parse();
        } else {
            cout << "ACPI: table " << i << ": parser not found\n";
        }
    }
}
