#include <stddef.h>
#include "terminal.h"
#include "cpuid.h"
#include "apic.h"
#include "idt.h"
#include "util.h"

union conv {
    char data[4 * 4 + 1];
    uint32_t conv[4];
} proc_name;

void kmain(void) {
    idt_init();
    terminal_initialize();
    _idt_enable_hardware_interrupts_asm();
    proc_name.data[16] = 0;
    proc_name.conv[0] = _get_cpuid_leaf_asm(0, 0, CPUID_EBX);
    proc_name.conv[1] = _get_cpuid_leaf_asm(0, 0, CPUID_EDX);
    proc_name.conv[2] = _get_cpuid_leaf_asm(0, 0, CPUID_ECX);
    terminal_printf("CPU primary name: %s\n", proc_name.data);
    terminal_printf("CPUID max leaf: %d\n", _get_cpuid_leaf_asm(0, 0, CPUID_EAX));
    terminal_printf("CPU time: %d\n", _get_time_asm());
    terminal_printf("CPU time: %d\n", _get_time_asm());
    terminal_printf("CPU time: %d\n", _get_time_asm());
    apic_base base = apic_get_base();
    apic_enable_spurious_interrupts(base);
    apic_init_lvt(base);
    apic_set_timer_divider(base, DIV_128);
    apic_alarm_timer(base, 0x1ffffff);
    terminal_printf("APIC is %s\n", apic_is_enabled() ? "enabled" : "disabled");
    terminal_printf("This processor is %s\n", apic_is_bsp() ? "BSP" : "AP");
    terminal_printf("APIC base: %x\n", base);
    uint32_t apic_version = apic_get_version_register(base);
    terminal_printf("APIC version: %x\n", apic_version & 0xff);
    terminal_printf("APIC # of LVT entries: %d\n", ((apic_version >> 16) & 0xff) + 1);
    terminal_printf("APIC errors: %x\n", apic_get_errors(base));
}