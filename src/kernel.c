#include <stddef.h>
#include "terminal.h"
#include "cpuid.h"
#include "apic.h"
#include "idt.h"
#include "acpi.h"
#include "ioapic.h"
#include "util.h"
#include "keyboard.h"

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
    _idt_enable_hardware_interrupts_asm();
    proc_name.data[16] = 0;
    proc_name.conv[0] = _get_cpuid_leaf_asm(0, 0, CPUID_EBX);
    proc_name.conv[1] = _get_cpuid_leaf_asm(0, 0, CPUID_EDX);
    proc_name.conv[2] = _get_cpuid_leaf_asm(0, 0, CPUID_ECX);
    terminal_printf("CPU primary name: %s\n", proc_name.data);
    terminal_printf("CPUID max leaf: %d\n", _get_cpuid_leaf_asm(0, 0, CPUID_EAX));
    apic_base apic_base = apic_get_base();
    apic_enable_spurious_interrupts(apic_base);
    apic_init_lvt(apic_base);
    apic_set_timer_divider(apic_base, DIV_128);
//    apic_alarm_timer(apic_base, 0xffffff);
    terminal_printf("APIC is %s\n", apic_is_enabled() ? "enabled" : "disabled");
    terminal_printf("This processor is %s\n", apic_is_bsp() ? "BSP" : "AP");
    terminal_printf("APIC base: %!x\n", apic_base);
    uint32_t apic_version = apic_get_version_register(apic_base);
    terminal_printf("APIC version: %x\n", apic_version & 0xff);
    terminal_printf("APIC # of LVT entries: %d\n", ((apic_version >> 16) & 0xff) + 1);
    terminal_printf("APIC errors: %x\n", apic_get_errors(apic_base));
    rsdp *rsdp_ptr = acpi_find_rsdp();
    if (!rsdp_ptr) {
        terminal_printf("ACPI RSDP not found!!!\n");
        return;
    }
    terminal_printf("ACPI RSDP address: %!x\n", rsdp_ptr);
    terminal_printf("ACPI RSDP revision: %!d\n", rsdp_ptr->revision);
    terminal_printf("ACPI OEMID: %[6]s\n", &rsdp_ptr->oem_id);
    rsdt *rsdt_ptr = rsdp_ptr->rsdt;
    terminal_printf("ACPI RSDT address: %!x\n", rsdt_ptr);
    terminal_printf("ACPI RSDT signature: %[4]s\n", &rsdt_ptr->header.signature);
    int acpi_tables_count = acpi_get_tables_count(rsdt_ptr);
    for (int i = 0; i < acpi_tables_count; ++i) {
        acpi_description_header *header = rsdt_ptr->tables[i];
        terminal_printf("ACPI table[%d] signature: %[4]s\n", i, &header->signature);
        if (acpi_is_apic_header(header)) {
            madt *apic_dt = (madt *) header;
            terminal_printf("ACPI table[%d] MADT APIC %s PC-AT compatible\n", i, apic_dt->flags ? "is" : "isn't");
            terminal_printf("ACPI table[%d] MADT structure size: %d\n", i, apic_dt->header.length);
            ics_header *ics = &apic_dt->ics[0];
            int j = 0;
            while ((uint32_t) ((char *) ics - (char *) apic_dt) < apic_dt->header.length) {
                switch (ics->type) {
                    case 0:
                        terminal_printf("ACPI table[%d] MADT ics table[%d] processor local APIC structure\n", i, j);
                        ics_processor_local_apic *apic = (ics_processor_local_apic *) ics;
                        terminal_printf("ACPI table[%d] MADT ics table[%d] processor UID %d -> APIC %d\n", i, j,
                                        apic->processor_uid, apic->apic_id);
                        break;
                    case 1:
                        terminal_printf("ACPI table[%d] MADT ics table[%d] IO APIC structure\n", i, j);
                        ics_io_apic *io_apic = (ics_io_apic *) ics;
                        terminal_printf("ACPI table[%d] MADT ics table[%d] IO APIC %d address: %!x\n", i, j,
                                        io_apic->io_apic_id, io_apic->io_apic);
                        global_io_apic_base = io_apic->io_apic;
                        break;
                    case 2:
                        terminal_printf("ACPI table[%d] MADT ics table[%d] interrupt source override structure\n", i,
                                        j);
                        ics_iso *iso = (ics_iso *) ics;
                        terminal_printf(
                                "ACPI table[%d] MADT ics table[%d] interrupt source override bus %d source %d -> global system interrupt %d\n",
                                i, j, iso->bus, iso->source, iso->global_system_interrupt);
                        break;
                    default:
                        terminal_printf("ACPI table[%d] MADT ics table[%d] type: unknown(%i)\n", i, j, *ics);
                        break;
                }
                ics = (ics_header *) (((char *) ics) + ics->length);
                j++;
            }
        }
    }
    io_apic_redirect_irq(global_io_apic_base, 1, apic_base, 35);
    keyboard_set_local_event_handler(local_k_handler);
}