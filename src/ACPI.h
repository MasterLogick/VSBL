#ifndef ACPI_H
#define ACPI_H

#include <stdint.h>
#include <stddef.h>
#include "Attributes.h"
#include "APIC.h"
#include "IOAPIC.h"

struct DescriptionHeader {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    char OEMTableID[8];
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;

    bool validate();
} PACKED;

struct RSDT;

struct RSDP {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    RSDT *rsdt;

    bool validate();
} PACKED;

struct RSDT : public DescriptionHeader {
    DescriptionHeader *tables[0];

    int getTablesCount();
} PACKED;

struct FADT : public DescriptionHeader {
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
    uint8_t FADT_MINOR_VERSION;
    uint64_t X_FIRMWARE_CTRL;
    uint64_t X_DSDT;

    void parse();
} PACKED;

struct ICSHeader {
    uint8_t type;
    uint8_t length;
} PACKED;

struct MADT : public DescriptionHeader {
    APIC *apicTable;
    uint32_t flags;
    ICSHeader ics[0];

    void parse();
} PACKED;

struct ProcessorLocalApic : public ICSHeader {
    uint8_t acpiProcessorID;
    uint8_t apicID;
    uint32_t flags;
} PACKED;

struct AcpiIOApic : public ICSHeader {
    uint8_t ioApicID;
    uint8_t reserved;
    IOAPIC *ioApic;
    uint32_t globalSystemInterruptBase;
} PACKED;

struct ISO : public ICSHeader {
    uint8_t bus;
    uint8_t source;
    uint32_t globalSystemInterrupt;
    uint16_t flags;
} PACKED;

bool parseACPITables();

extern RSDP *GlobalRSDP;
extern RSDT *GlobalRSDT;
extern FADT *GlobalFADT;
extern MADT *GlobalMADT;

#endif //ACPI_H
