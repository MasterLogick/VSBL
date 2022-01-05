#ifndef IDT_C
#define IDT_C

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

#define IDT_DESCRIPTORS_COUNT 36

struct idt_descriptor {
    uint16_t entry[4];
};
typedef struct idt_descriptor idt_descriptor;

void idt_init(void);

void _idt_load_table_asm(uint16_t size, void *idt_ptr);

void _idt_enable_hardware_interrupts_asm(void);

void _idt_disable_hardware_interrupts_asm(void);

void _idt_int_3_handler_asm(void);

void _idt_int_fallback_handler_asm(void);

void _idt_int_test_handler_asm(void);

void _idt_int_DF_handler_asm(void);

void _idt_int_apic_timer_handler_asm(void);

void _idt_int_apic_error_handler_asm(void);

void _idt_int_keyboard_handler_asm(void);

#ifdef __cplusplus
}
#endif

#endif //IDT_C