BITS 32
section .secondary_bootsection

extern stack_top
extern kmain

global _protected_mode_entry_asm
_protected_mode_entry_asm:
    mov ax, 0x10
    mov es, ax
    mov ss, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov esp, stack_top
    cli
    call kmain
    jmp $
.end:
size _protected_mode_entry_asm _protected_mode_entry_asm.end - _protected_mode_entry_asm
