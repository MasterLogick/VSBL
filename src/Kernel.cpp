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

union conv {
    char data[4 * 4 + 1];
    uint32_t conv[4];
} proc_name;

void local_k_handler(uint8_t scancode, char key, uint32_t event) {
    if (event == KEYBOARD_KEY_PRESSED || event == KEYBOARD_KEY_REPEAT) {
        terminal_printf("%c", key);
    }
    (void) scancode;
    (void) event;
}

void kmain() {
    CallGlobalConstructors();
    initIDT();
    terminal_initialize();
    GlobalMemoryLayout.extractConventionalBlocks();
    _idt_enable_hardware_interrupts_asm();
    proc_name.data[16] = 0;
    proc_name.conv[0] = _get_cpuid_leaf_asm(0, 0, CPUID_EBX);
    proc_name.conv[1] = _get_cpuid_leaf_asm(0, 0, CPUID_EDX);
    proc_name.conv[2] = _get_cpuid_leaf_asm(0, 0, CPUID_ECX);
    terminal_printf("CPUID: CPU primary name: %s\n", proc_name.data);
    terminal_printf("CPUID: max leaf: %d\n", _get_cpuid_leaf_asm(0, 0, CPUID_EAX));
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
    KeyboardSetLocalEventHandler(local_k_handler);
    for (int i = 0; i < GlobalMemoryLayout.conventionalBlocksCount; ++i) {
        terminal_printf("PMM: range %d: start: %!x%!x, length: %!x%!x\n",
                        i, GlobalMemoryLayout.conventionalMemory[i].address,
                        GlobalMemoryLayout.conventionalMemory[i].length);
    }
}