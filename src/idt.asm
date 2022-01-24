BITS 64
section .text

%macro macro_pushad 0
    push rax
    push rcx
    push rdx
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
%endmacro

%macro macro_popad 0
   pop r11
   pop r10
   pop r9
   pop r8
   pop rdi
   pop rsi
   pop rdx
   pop rcx
   pop rax
%endmacro

extern _Z15terminal_printfPKcz
extern _ZN4APIC3eoiEv
extern _ZN4APIC7getAPICEv
extern _Z26KeyboardGlobalEventHandlerv
extern _Z20terminal_print_int64mib

; void _idt_load_table_asm(uint16_t size, void *idt_ptr)
global _idt_load_table_asm
_idt_load_table_asm:
    sub rsp, 10
    mov [rsp], di
    mov [rsp + 2], rsi
    lidt [rsp]
    add rsp, 10
    ret
.end:
size _idt_load_table_asm _idt_load_table_asm.end - _idt_load_table_asm

; void _idt_int_3_handler_asm()
global _idt_int_3_handler_asm
_idt_int_3_handler_asm:
    macro_pushad
    mov rdi, aInt3Msg
    call _Z15terminal_printfPKcz
    macro_popad
    iretq
.end:
size _idt_int_3_handler_asm _idt_int_3_handler_asm.end - _idt_int_3_handler_asm

; void _idt_int_fallback_handler_asm()
global _idt_int_fallback_handler_asm
_idt_int_fallback_handler_asm:
;    macro_pushad
    mov rdi, aIntFallbackMsg1
    call _Z15terminal_printfPKcz
    mov rdi, [rsp + 8]
    mov rsi, 16
    mov rdx, 1
    call _Z20terminal_print_int64mib
    mov rdi, aIntFallbackMsg2
    call _Z15terminal_printfPKcz
    jmp $
;    macro_popad
;    iretq
.end:
size _idt_int_fallback_handler_asm _idt_int_fallback_handler_asm.end - _idt_int_fallback_handler_asm

; void _idt_enable_hardware_interrupts_asm()
global _idt_enable_hardware_interrupts_asm
_idt_enable_hardware_interrupts_asm:
    sti
    ret
.end:
size _idt_enable_hardware_interrupts_asm _idt_enable_hardware_interrupts_asm.end - _idt_enable_hardware_interrupts_asm

; void _idt_disable_hardware_interrupts_asm()
global _idt_disable_hardware_interrupts_asm
_idt_disable_hardware_interrupts_asm:
    cli
    ret
.end:
size _idt_disable_hardware_interrupts_asm _idt_disable_hardware_interrupts_asm.end - _idt_disable_hardware_interrupts_asm

; void _idt_int_test_handler_asm()
global _idt_int_test_handler_asm
_idt_int_test_handler_asm:
    macro_pushad
    mov rdi, aIntTestMsg
    call _Z15terminal_printfPKcz
    macro_popad
    iretq
.end:
size _idt_int_test_handler_asm _idt_int_test_handler_asm.end - _idt_int_test_handler_asm

; void _idt_int_DF_handler_asm()
global _idt_int_DF_handler_asm
_idt_int_DF_handler_asm:
    macro_pushad
;    push rax
;    push rbx
;    push rcx
    mov rdi, aIntDFMsg
    call _Z15terminal_printfPKcz
;    add rsp, 8*3
    macro_popad
    iretq
.end:
size _idt_int_DF_handler_asm _idt_int_DF_handler_asm.end - _idt_int_DF_handler_asm

; void _idt_int_apic_timer_handler_asm()
global _idt_int_apic_timer_handler_asm
_idt_int_apic_timer_handler_asm:
    macro_pushad
    mov rdi, aIntTimerMsg
    call _Z15terminal_printfPKcz
    call _ZN4APIC7getAPICEv
    mov rdi, rax
    call _ZN4APIC3eoiEv
    macro_popad
    iretq
.end:
size _idt_int_apic_timer_handler_asm _idt_int_apic_timer_handler_asm.end - _idt_int_apic_timer_handler_asm

; void _idt_int_apic_error_handler_asm()
global _idt_int_apic_error_handler_asm
_idt_int_apic_error_handler_asm:
    macro_pushad
    call _ZN4APIC7getAPICEv
    push rax
    add rax, 0x280
    mov rsi, qword [rax]
    mov rdi, aIntAPICErrorMsg
    call _Z15terminal_printfPKcz
    pop rdi
    call _ZN4APIC3eoiEv
    macro_popad
    iretq
.end:
size _idt_int_apic_error_handler_asm _idt_int_apic_error_handler_asm.end - _idt_int_apic_error_handler_asm

; void _idt_int_keyboard_handler_asm()
global _idt_int_keyboard_handler_asm
_idt_int_keyboard_handler_asm:
    macro_pushad
    call _Z26KeyboardGlobalEventHandlerv
    call _ZN4APIC7getAPICEv
    mov rdi, rax
    call _ZN4APIC3eoiEv
    macro_popad
    iretq
.end:
size _idt_int_keyboard_handler_asm _idt_int_keyboard_handler_asm.end - _idt_int_keyboard_handler_asm

; void _idt_int_page_fault_asm()
global _idt_int_page_fault_asm
_idt_int_page_fault_asm:
    macro_pushad
    mov rbx, cr2
    mov rdi, aIntPageFault1
    call _Z15terminal_printfPKcz
    mov rdi, rbx
    mov rsi, 16
    mov rdx, 1
    call _Z20terminal_print_int64mib
    mov rdi, aIntPageFault2
    call _Z15terminal_printfPKcz
    mov rdi, [rsp + 8 * 10]
    mov rsi, 16
    mov rdx, 1
    call _Z20terminal_print_int64mib
    mov rdi, aIntPageFault3
    call _Z15terminal_printfPKcz
    mov rdi, [rsp + 8 * 9]
    mov rsi, 16
    mov rdx, 1
    call _Z20terminal_print_int64mib
    mov rdi, aIntPageFault4
    call _Z15terminal_printfPKcz
    jmp $
    macro_popad
    iretq
.end:
size _idt_int_page_fault_asm _idt_int_page_fault_asm.end - _idt_int_page_fault_asm

; void _idt_int_invalid_opcode_asm()
global _idt_int_invalid_opcode_asm
_idt_int_invalid_opcode_asm:
    macro_pushad
    mov rdi, aIntInvalidOpcode
    call _Z15terminal_printfPKcz
    macro_popad
    iretq

section .rodata

aIntFallbackMsg1 db "Fallback interrupt handler reached at: 0x", 0
aIntFallbackMsg2 db ". Freezing...", 0x0a, 0;, "   EDI=0x%!x", 0x0a, "   ESI=0x%!x", 0x0a,\
;    "   EBP=0x%!x", 0x0a, "   ESP=0x%!x", 0x0a, "   EBX=0x%!x", 0x0a, "   EDX=0x%!x", 0x0a, "   ECX=0x%!x", 0x0a,\
;    "   EAX=0x%!x", 0x0a, "   EIP=0x%!x", 0x0a, "    CS=0x%!x", 0x0a, "EFLAGS=0x%!x", 0x0a, 0
aInt3Msg db "Breakpoint at address: %x", 0x0a, 0
aIntTestMsg db "Test msg EIP=%!x CS=%!x EFLAGS=%!x", 0x0a, 0
aIntDFMsg db 0x0a, "Abort #DF at EIP=%!x CS=%!x EFLAGS=%!x", 0x0a, 0
aIntTimerMsg db "Timer reached zero", 0x0a, 0
aIntAPICErrorMsg db "APIC errors: %x", 0x0a, 0
aIntPageFault1 db "Page fault at linear address: 0x", 0
aIntPageFault2 db 0xa, "RIP: 0x", 0
aIntPageFault3 db 0xa, "Error code: 0x", 0
aIntPageFault4 db 0xa, 0
aIntInvalidOpcode db "Invalid opcode interrupt", 0xa, 0