#ifndef FADT_H
#define FADT_H

#include <stdint.h>
#include "../../Attributes.h"
#include "DescriptionHeader.h"
#include "../GenericAddressStructure.h"
#include "DSDT.h"

struct FADT : public DescriptionHeader {
    uint32_t FIRMWARE_CTRL;
    uint32_t DSDTptr;
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
    GenericAddressStructure RESET_REG;
    uint8_t RESET_VALUE;
    uint16_t ARM_BOOT_ARCH;
    uint8_t FADT_MINOR_VERSION;
    void *X_FIRMWARE_CTRL;
    DSDT *X_DSDT;
    GenericAddressStructure X_PM1a_EVT_BLK;
    GenericAddressStructure X_PM1b_EVT_BLK;
    GenericAddressStructure X_PM1a_CNT_BLK;
    GenericAddressStructure X_PM1b_CNT_BLK;
    GenericAddressStructure X_PM2_CNT_BLK;
    GenericAddressStructure X_PM_TMR_BLK;
    GenericAddressStructure X_GPE0_BLK;
    GenericAddressStructure X_GPE1_BLK;
    GenericAddressStructure SLEEP_CONTROL_REG;
    GenericAddressStructure SLEEP_STATUS_REG;
    uint64_t Hypervisor_Vendor_Identity;

    void parse();
} PACKED;

extern FADT *GlobalFADT;

#endif //FADT_H
