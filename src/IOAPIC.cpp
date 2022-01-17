#include "IOAPIC.h"
#include "terminal.h"

#define VERSION_REGISTER 0x1
#define IO_APIC_RED_TBL_LOW(x) (0x10 + 2 * (x))
#define IO_APIC_RED_TBL_HIGH(x) (0x10 + 2 * (x) + 1)

IOAPIC *GlobalIOAPIC;

uint32_t IOAPIC::readRegister(uint8_t index) {
    base() = index;
    return data();
}

void IOAPIC::writeRegister(uint8_t index, uint32_t value) {
    base() = index;
    data() = value;
}

void IOAPIC::redirectIRQ(uint8_t irq, APIC *apic, uint8_t vector) {
    writeRegister(IO_APIC_RED_TBL_HIGH(irq), (apic->getID() & 0xf) << 24);
    writeRegister(IO_APIC_RED_TBL_LOW(irq), vector);
    terminal_printf("IO APIC: redirect IRQ%d to APIC %d vector %d\n", irq, apic->getID() & 0xf, vector);
}

uint8_t IOAPIC::getRedirectionEntryCount() {
    return readRegister(VERSION_REGISTER) >> 16;
}
