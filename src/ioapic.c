#include "ioapic.h"
#include "terminal.h"

uint32_t io_apic_read_reg(io_apic_base base, uint8_t index) {
    base[0] = index;
    return *(uint32_t *) (base + 0x10);
}

void io_apic_write_reg(io_apic_base base, uint8_t index, uint32_t data) {
    base[0] = index;
    *(uint32_t *) (base + 0x10) = data;
}

void io_apic_redirect_irq(io_apic_base base, uint8_t irq, apic_base apic_b, uint8_t vector) {
    terminal_printf("Redirect IRQ%d to APIC %d Vector %d\n", irq, apic_get_id(apic_b) & 0xf, vector);
    io_apic_write_reg(base, IO_APIC_RED_TBL_HIGH(irq), (apic_get_id(apic_b) & 0xf) << 24);
    io_apic_write_reg(base, IO_APIC_RED_TBL_LOW(irq), vector);
}