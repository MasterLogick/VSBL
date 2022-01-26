#ifndef ACPIIOAPIC_H
#define ACPIIOAPIC_H

#include <stdint.h>
#include "../../../Attributes.h"
#include "ICSHeader.h"
#include "../../../IOAPIC.h"

struct AcpiIOApic : public ICSHeader {
    uint8_t ioApicID;
    uint8_t reserved;
    IOAPIC *ioApic;
    uint32_t globalSystemInterruptBase;
} PACKED;

#endif //ACPIIOAPIC_H
