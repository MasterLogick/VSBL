#include "APIC.h"
#include "MSR.h"

#define IA32_APIC_BASE_MSR 0x1B

APIC *APIC::getAPIC() {
    return (APIC *) (_msr_get_asm(IA32_APIC_BASE_MSR) & 0xfffff000);
}

bool isAPICEnabled() {
    return _msr_get_asm(IA32_APIC_BASE_MSR) & (1 << 11);
}

bool isAPICBSP() {
    return _msr_get_asm(IA32_APIC_BASE_MSR) & (1 << 8);
}

uint8_t APIC::getID() {
    return ID() >> 24;
}

void APIC::eoi() {
    EOI() = 0;
}

void APIC::initLVT() {
    lvtTimer() = 32;
    lvtError() = 34;
}

void APIC::setTimerDivider(TimerDivider divider) {
    divideConfiguration() = divider;
}

void APIC::alarmTimer(uint32_t time) {
    initialCount() = time;
}

void APIC::enableSpuriousInterrupts() {
    spuriousInterruptVector() = 0x121;
}
