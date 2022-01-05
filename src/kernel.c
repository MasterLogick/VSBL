#include <stddef.h>
#include "terminal.h"
#include "cpuid.h"
#include "apic.h"
#include "idt.h"
#include "acpi.h"
#include "ioapic.h"
#include "util.h"
#include "keyboard.h"
#include "ps2.h"
#include "physical_memory_manager.h"

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

void kmain(void) {
    idt_init();
    terminal_initialize();
    pmm_extract_usable_blocks();
    _idt_enable_hardware_interrupts_asm();
    proc_name.data[16] = 0;
    proc_name.conv[0] = _get_cpuid_leaf_asm(0, 0, CPUID_EBX);
    proc_name.conv[1] = _get_cpuid_leaf_asm(0, 0, CPUID_EDX);
    proc_name.conv[2] = _get_cpuid_leaf_asm(0, 0, CPUID_ECX);
    terminal_printf("CPUID: CPU primary name: %s\n", proc_name.data);
    terminal_printf("CPUID: max leaf: %d\n", _get_cpuid_leaf_asm(0, 0, CPUID_EAX));
    apic_base apic_base = apic_get_base();
    apic_enable_spurious_interrupts(apic_base);
    apic_init_lvt(apic_base);
    apic_set_timer_divider(apic_base, DIV_128);
//    apic_alarm_timer(apic_base, 0xffffff);
    terminal_printf("APIC: %s\n", apic_is_enabled() ? "enabled" : "disabled");
    terminal_printf("APIC: This processor is %s\n", apic_is_bsp() ? "BSP" : "AP");
    terminal_printf("APIC: base: %!x\n", apic_base);
    uint32_t apic_version = apic_get_version_register(apic_base);
    terminal_printf("APIC: version: %x\n", apic_version & 0xff);
    terminal_printf("APIC: # of LVT entries: %d\n", ((apic_version >> 16) & 0xff) + 1);
    terminal_printf("APIC: errors: %x\n", apic_get_errors(apic_base));
    acpi_parse_tables();
    if (!ps2_init()) {
        terminal_printf("PS/2: initialisation failed\n");
    }
    io_apic_redirect_irq(global_io_apic_base, 1, apic_base, 35);
    keyboard_set_local_event_handler(local_k_handler);
    for (int i = 0; i < pmm_conventional_blocks_count; ++i) {
        terminal_printf("PMM: range %d: start: %!x%!x, length: %!x%!x\n",
                        i, pmm_conventional_blocks[i].base_addr, pmm_conventional_blocks[i].length);
    }
}