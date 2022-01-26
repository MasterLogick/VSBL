BITS 64
section .long_mode_section

extern _Z5kmainv
extern GDT64_Data
extern stack_top
extern GDT64_TSS

global _long_mode_entry_asm
_long_mode_entry_asm:

    cli
    mov ax, GDT64_Data
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    mov rsp, stack_top
    push $
    mov rbp, 0
    push rbp
    mov ax, GDT64_TSS
    ltr ax
    jmp _Z5kmainv

.end:
size _long_mode_entry_asm _long_mode_entry_asm.end - _long_mode_entry_asm

