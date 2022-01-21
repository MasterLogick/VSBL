#include <stddef.h>
#include "terminal.h"
#include "CpuID.h"
#include "APIC.h"
#include "IDT.h"
#include "ACPI.h"
#include "IOAPIC.h"
#include "util.h"
#include "Keyboard.h"
#include "ps2.h"
#include "PhysicalMemoryManager.h"
#include "GlobalConstructorCaller.h"
#include "Attributes.h"

void local_k_handler(uint8_t scancode, char key, uint32_t event) {
    if (event == KEYBOARD_KEY_PRESSED || event == KEYBOARD_KEY_REPEAT) {
        terminal_printf("%c", key);
    }
    (void) scancode;
    (void) event;
}

NORETURN void kmain() {
    initIDT();
    terminal_initialize();
    GlobalPMM.extractConventionalBlocks();
    CallGlobalConstructors();
    APIC *apic = getAPIC();
    apic->enableSpuriousInterrupts();
    apic->initLVT();
    apic->setTimerDivider(DIV_128);
//    apic_alarm_timer(apic_base, 0xffffff);
    terminal_printf("APIC: %s\n", isAPICEnabled() ? "enabled" : "disabled");
    terminal_printf("APIC: This processor is %s\n", isAPICBSP() ? "BSP" : "AP");
    terminal_printf("APIC: base: %!x\n", apic);
    uint32_t apic_version = apic->version();
    terminal_printf("APIC: version: %x\n", apic_version & 0xff);
    terminal_printf("APIC: # of LVT entries: %d\n", ((apic_version >> 16) & 0xff) + 1);
    terminal_printf("APIC: errors: %x\n", apic->errorStatus());
    parseACPITables();
    if (!ps2_init()) {
        terminal_printf("PS/2: initialisation failed\n");
    }
    GlobalIOAPIC->redirectIRQ(1, apic, 35);
    terminal_printf("IO APIC: redirection entry count: %d\n", GlobalIOAPIC->getRedirectionEntryCount());
    KeyboardSetLocalEventHandler(local_k_handler);
    terminal_printf("APIC: RSDP version %d\n", GlobalRSDP->revision);
    terminal_printf("CPUID: max extended value: %x\n", _get_cpuid_leaf_asm(80000008, 0, CPUID_EAX) & 0xff);
    while (true);
}