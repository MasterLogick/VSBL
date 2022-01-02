#include "apic.h"
#include "msr.h"

#define IA32_APIC_BASE_MSR 0x1B
#define APIC_LVT_TIMER_REGISTER 0x320
#define APIC_LVT_ERROR_REGISTER 0x370
#define APIC_VERSION_REGISTER 0x30
#define APIC_CURRENT_COUNT_REGISTER 0x390
#define APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER 0xf0
#define APIC_DIVIDE_CONFIGURATION_REGISTER 0x3e0
#define APIC_INITIAL_COUNT_REGISTER 0x380
#define APIC_ID_REGISTER 0x20
#define APIC_EOI_REGISTER 0xb0

apic_base apic_get_base() {
    return (apic_base) (uintptr_t) (_msr_get_asm(IA32_APIC_BASE_MSR) & 0xfffff000);
}

bool apic_is_enabled(void) {
    return _msr_get_asm(IA32_APIC_BASE_MSR) & (1 << 11);
}

bool apic_is_bsp(void) {
    return _msr_get_asm(IA32_APIC_BASE_MSR) & (1 << 8);
}

uint32_t apic_get_version_register(apic_base base) {
    return *((uint32_t *) (base + APIC_VERSION_REGISTER));
}

uint8_t apic_get_id(apic_base base) {
    return *((uint32_t *) (base + APIC_ID_REGISTER)) >> 24;
}

void apic_eoi(apic_base base) {
    *((uint32_t *) (base + APIC_EOI_REGISTER)) = 0;
}

void apic_init_lvt(apic_base base) {
    *((uint32_t *) (base + APIC_LVT_TIMER_REGISTER)) = 32;
    *((uint32_t *) (base + APIC_LVT_ERROR_REGISTER)) = 34;
}

void apic_set_timer_divider(apic_base base, timer_divider divider) {
    *((uint32_t *) (base + APIC_DIVIDE_CONFIGURATION_REGISTER)) = divider;
}

void apic_alarm_timer(apic_base base, uint32_t time) {
    *((uint32_t *) (base + APIC_INITIAL_COUNT_REGISTER)) = time;
}

uint32_t apic_get_current_count_register(apic_base base) {
    return *((uint32_t *) (base + APIC_CURRENT_COUNT_REGISTER));
}

void apic_enable_spurious_interrupts(apic_base base) {
    *((uint32_t *) (base + APIC_SPURIOUS_INTERRUPT_VECTOR_REGISTER)) = 0x121;
}

uint32_t apic_get_errors(apic_base base) {
    return *((uint32_t *) (base + 0x280));
}