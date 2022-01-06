BITS 32
section .text

extern _Z15terminal_printfPKcz
extern _ZN4APIC3eoiEv
extern _Z7getAPICv
extern _Z26KeyboardGlobalEventHandlerv

; void _idt_load_table_asm(uint16_t size, void *idt_ptr)
global _idt_load_table_asm
_idt_load_table_asm:
    sub esp, 8
    mov ax, [esp + 8 + 4]
    dec ax
    mov [esp], ax
    mov eax, [esp + 8 + 8]
    mov [esp + 2], eax
    lidt [esp]
    add esp, 8
    ret
.end:
size _idt_load_table_asm _idt_load_table_asm.end - _idt_load_table_asm

; void _idt_int_3_handler_asm(void)
global _idt_int_3_handler_asm
_idt_int_3_handler_asm:
    pushad
    push aInt3Msg
    call _Z15terminal_printfPKcz
    add esp, 4
    popad
    iretd
.end:
size _idt_int_3_handler_asm _idt_int_3_handler_asm.end - _idt_int_3_handler_asm

; void _idt_int_fallback_handler_asm(void)
global _idt_int_fallback_handler_asm
_idt_int_fallback_handler_asm:
    pushad
    push aIntFallbackMsg
    call _Z15terminal_printfPKcz
    add esp, 4
    jmp $
    popad
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

; void _idt_disable_hardware_interrupts_asm(void)
global _idt_disable_hardware_interrupts_asm
_idt_disable_hardware_interrupts_asm:
    cli
    ret
.end:
size _idt_disable_hardware_interrupts_asm _idt_disable_hardware_interrupts_asm.end - _idt_disable_hardware_interrupts_asm

; void _idt_int_test_handler_asm(void)
global _idt_int_test_handler_asm
_idt_int_test_handler_asm:
    pushad
    push aIntTestMsg
    call _Z15terminal_printfPKcz
    add esp, 4
    popad
    iretd
.end:
size _idt_int_test_handler_asm _idt_int_test_handler_asm.end - _idt_int_test_handler_asm

; void _idt_int_DF_handler_asm(void)
global _idt_int_DF_handler_asm
_idt_int_DF_handler_asm:
    pushad
    mov eax, [esp + 8*4 + 2*4]
    mov ebx, [esp + 8*4 + 1*4]
    mov ecx, [esp + 8*4]
    push eax
    push ebx
    push ecx
    push aIntDFMsg
    call _Z15terminal_printfPKcz
    add esp, 4*4
    popad
    iretd
.end:
size _idt_int_DF_handler_asm _idt_int_DF_handler_asm.end - _idt_int_DF_handler_asm

; void _idt_int_apic_timer_handler_asm(void)
global _idt_int_apic_timer_handler_asm
_idt_int_apic_timer_handler_asm:
    pushad
    push aIntTimerMsg
    call _Z15terminal_printfPKcz
    call _Z7getAPICv
    mov [esp], eax
    call _ZN4APIC3eoiEv
    add esp, 4
    popad
    iretd
.end:
size _idt_int_apic_timer_handler_asm _idt_int_apic_timer_handler_asm.end - _idt_int_apic_timer_handler_asm

; void _idt_int_apic_error_handler_asm(void)
global _idt_int_apic_error_handler_asm
_idt_int_apic_error_handler_asm:
    pushad
    call _Z7getAPICv
    add eax, 0x280
    push dword [eax]
    push aIntAPICErrorMsg
    call _Z15terminal_printfPKcz
    call _Z7getAPICv
    mov [esp], eax
    call _ZN4APIC3eoiEv
    add esp, 8
    popad
    iretd
.end:
size _idt_int_apic_error_handler_asm _idt_int_apic_error_handler_asm.end - _idt_int_apic_error_handler_asm

; void _idt_int_keyboard_handler_asm(void)
global _idt_int_keyboard_handler_asm
_idt_int_keyboard_handler_asm:
    pushad
    call _Z26KeyboardGlobalEventHandlerv
    call _Z7getAPICv
    push eax
    call _ZN4APIC3eoiEv
    add esp, 4
    popad
    iretd
.end:
size _idt_int_keyboard_handler_asm _idt_int_keyboard_handler_asm.end - _idt_int_keyboard_handler_asm

section .data

aIntFallbackMsg db "Fallback interrupt handler reached. Freezing...", 0x0a, "   EDI=0x%!x", 0x0a, "   ESI=0x%!x", 0x0a,\
    "   EBP=0x%!x", 0x0a, "   ESP=0x%!x", 0x0a, "   EBX=0x%!x", 0x0a, "   EDX=0x%!x", 0x0a, "   ECX=0x%!x", 0x0a,\
    "   EAX=0x%!x", 0x0a, "   EIP=0x%!x", 0x0a, "    CS=0x%!x", 0x0a, "EFLAGS=0x%!x", 0x0a, 0
aInt3Msg db "Breakpoint at address: %x", 0x0a, 0
aIntTestMsg db "Test msg EIP=%!x CS=%!x EFLAGS=%!x", 0x0a, 0
aIntDFMsg db 0x0a, "Abort #DF at EIP=%!x CS=%!x EFLAGS=%!x", 0x0a, 0
aIntTimerMsg db "Timer reached zero", 0x0a, 0
aIntAPICErrorMsg db "APIC errors: %x", 0x0a, 0