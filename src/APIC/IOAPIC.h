#ifndef IOAPIC_H
#define IOAPIC_H

#include <stdint.h>
#include "APIC.h"

class IOAPIC {
    APICRegister base;
    APICRegister data;

    uint32_t readRegister(uint8_t index);

    void writeRegister(uint8_t index, uint32_t value);

public:
    void redirectIRQ(uint8_t irq, APIC *apic, uint8_t vector);

    uint8_t getRedirectionEntryCount();
};

extern IOAPIC *GlobalIOAPIC;

#endif //IOAPIC_H
