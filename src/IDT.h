#ifndef IDT_C
#define IDT_C

#include <stdint.h>
#include "Attributes.h"

struct IDTDescriptor {
    uint16_t entry[8];

public:
    IDTDescriptor();

    explicit IDTDescriptor(void (*handler)());
} PACKED;

void initIDT();

#ifdef __cplusplus
extern "C" {
#endif

void _idt_load_table_asm(uint16_t size, void *idt_ptr);

void _idt_enable_hardware_interrupts_asm();

void _idt_disable_hardware_interrupts_asm();

void _idt_int_3_handler_asm();

void _idt_int_fallback_handler_asm();

void _idt_int_test_handler_asm();

void _idt_int_DF_handler_asm();

void _idt_int_apic_timer_handler_asm();

void _idt_int_apic_error_handler_asm();

void _idt_int_keyboard_handler_asm();

void _idt_int_page_fault_asm();

void _idt_int_invalid_opcode_asm();

#ifdef __cplusplus
}
#endif

#endif //IDT_C