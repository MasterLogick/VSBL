BITS 32
section .text

extern terminal_printf

; void _idt_load_table_asm(uint16_t size, void *idt_ptr)
global _idt_load_table_asm
_idt_load_table_asm:
    sub esp, 8
    mov ax, [esp+8+4]
    dec ax
    mov [esp], ax
    mov eax, [esp+8+8]
    mov [esp+2], eax
    lidt [esp]
    add esp, 8
    ret
.end:
size _idt_load_table_asm _idt_load_table_asm.end - _idt_load_table_asm

; void _idt_int_3_handler_asm(void)
global _idt_int_3_handler_asm
_idt_int_3_handler_asm:
    push aInt3Msg
    call terminal_printf
    add esp, 4
;    jmp $
    iretd
.end:
size _idt_int_3_handler_asm _idt_int_3_handler_asm.end - _idt_int_3_handler_asm

; void _idt_int_fallback_handler_asm(void)
global _idt_int_fallback_handler_asm
_idt_int_fallback_handler_asm:
    push aIntFallbackMsg
    call terminal_printf
    add esp, 4
    jmp $
    iretd
.end:
size _idt_int_fallback_handler_asm _idt_int_fallback_handler_asm.end - _idt_int_fallback_handler_asm

; void _idt_enable_hardware_interrupts_asm(void)
global _idt_enable_hardware_interrupts_asm
_idt_enable_hardware_interrupts_asm:
    sti
    ret
.end:
size _idt_enable_hardware_interrupts_asm _idt_enable_hardware_interrupts_asm.end - _idt_enable_hardware_interrupts_asm

; void _idt_int_test_handler_asm(void)
global _idt_int_test_handler_asm
_idt_int_test_handler_asm:
    push aIntTestMsg
    call terminal_printf
    add esp, 4
    iretd
.end:
size _idt_int_test_handler_asm _idt_int_test_handler_asm.end - _idt_int_test_handler_asm

; void _idt_int_DF_handler_asm(void)
global _idt_int_DF_handler_asm
_idt_int_DF_handler_asm:
    push aIntDFMsg
    call terminal_printf
    add esp, 4
    iretd
.end:
size _idt_int_DF_handler_asm _idt_int_DF_handler_asm.end - _idt_int_DF_handler_asm

; void _idt_int_apic_timer_handler_asm(void)
global _idt_int_apic_timer_handler_asm
_idt_int_apic_timer_handler_asm:
    push aIntTimerMsg
    call terminal_printf
    add esp, 4
    iretd
.end:
size _idt_int_apic_timer_handler_asm _idt_int_apic_timer_handler_asm.end - _idt_int_apic_timer_handler_asm


extern apic_get_base
extern apic_get_errors

; void _idt_int_apic_error_handler_asm(void)
global _idt_int_apic_error_handler_asm
_idt_int_apic_error_handler_asm:
    call apic_get_base
    push eax
    call apic_get_errors
    mov [esp], eax
    push aIntAPICErrorMsg
    call terminal_printf
    add esp, 8
    iretd
.end:
size _idt_int_apic_error_handler_asm _idt_int_apic_error_handler_asm.end - _idt_int_apic_error_handler_asm

section .data

aIntFallbackMsg db "Interrupt at address: %x. Freezing...", 0x0a, 0
aInt3Msg db "Breakpoint at address: %x", 0x0a, 0
aIntTestMsg db "Test msg EIP=%x CS=%x EFLAGS=%x", 0x0a, 0
aIntDFMsg db "Abort #DF at EIP=%x CS=%x EFLAGS=%x", 0x0a, 0
aIntTimerMsg db "Timer reached zero", 0x0a, 0
aIntAPICErrorMsg db "APIC errors: %x", 0x0a, 0