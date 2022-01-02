#ifndef IOAPIC_H
#define IOAPIC_H

#include <stdint.h>
#include "apic.h"

typedef uint8_t *io_apic_base;

io_apic_base global_io_apic_base;

#define IO_APIC_ID 0x0
#define IO_APIC_VERSION 0x1
#define IO_APIC_ARBITRATION_ID 0x2
#define IO_APIC_RED_TBL_LOW(x) (0x10 + 2 * (x))
#define IO_APIC_RED_TBL_HIGH(x) (0x10 + 2 * (x) + 1)

uint32_t io_apic_read_reg(io_apic_base base, uint8_t index);

void io_apic_write_reg(io_apic_base base, uint8_t index, uint32_t data);

void io_apic_redirect_irq(io_apic_base base, uint8_t irq, apic_base apic_b, uint8_t vector);

#endif //IOAPIC_H
