;Secondary bootloader errors:
;C - CPUID command is unavailable
;E - CPUID extended function limit
;X - x64 architecture is not supported

BITS 32
section .protected_mode_section

extern stack_top
extern _long_mode_entry_asm

global _protected_mode_entry_asm
_protected_mode_entry_asm:

    ;setup segment registers
    mov ax, 0x10
    mov es, ax
    mov ss, ax
    mov ds, ax
    mov fs, ax
    mov gs, ax
    mov esp, stack_top
    mov ebp, stack_top

    ;check CPUID availability
    pushfd
    pop eax
    bts eax, 21
    push eax
    popfd
    pushfd
    pop ebx
    bt ebx, 21
    mov al, 'C'
    jnc .err
    btr ebx, 21
    push ebx
    popfd
    pushfd
    pop ebx
    bt ebx, 21
    mov al, 'C'
    jc .err
    bts ebx, 21
    push ebx
    popfd

    ;check 64 bit arch
    mov eax, 0x80000000
    cpuid
    cmp eax, 0x80000001
    mov al, 'E'
    jle .err
    mov eax, 0x80000001
    cpuid
    bt edx, 29
    mov al, 'X'
    jnc .err

    ;enable PAE
    mov eax, 0x20
    mov cr4, eax

    ;load paging tables
    mov ebx, PDE_table
    or ebx, 00000111b
    mov eax, PDPTE_table
    mov [eax], ebx
    or eax, 00000111b
    mov ebx, PML4_table
    mov dword [ebx], eax

    ;load PML4 table address
    mov cr3, ebx

    ;enable long mode
    mov ecx, 0xC0000080
    rdmsr
    bts eax, 8
    wrmsr

    ;enable paging
    mov eax, cr0
    bts eax, 31
    mov cr0, eax

    lgdt [GDT64_ptr]
    jmp GDT64_Code:_long_mode_entry_asm

.err:
    mov [0xb8000], al
    jmp $

.end:
size _protected_mode_entry_asm _protected_mode_entry_asm.end - _protected_mode_entry_asm

section .data

align 16
GDT64_ptr:
dw GDT64.end - GDT64 -1
dq GDT64

; Access bits
PRESENT        equ 1 << 7
NOT_SYS        equ 1 << 4
EXEC           equ 1 << 3
DC             equ 1 << 2
RW             equ 1 << 1
SYS_TSS_AVL    equ 1001b

; Flags bits
GRANULARITY_4K equ 1 << 7
SZ_32          equ 1 << 6
LONG_MODE      equ 1 << 5

align 16
global GDT64
GDT64:
global GDT64_Null
GDT64_Null equ $ - GDT64
    dq 0
global GDT64_Code
GDT64_Code equ $ - GDT64
    dw 0xFFFF                                   ; Limit (low, bits 0-15)
    dw 0                                        ; Base (low, bits 0-15)
    db 0                                        ; Base (mid, bits 16-23)
    db PRESENT | NOT_SYS | EXEC | RW            ; Access
    db GRANULARITY_4K | LONG_MODE | 0xF         ; Flags & Limit (high, bits 16-19)
    db 0                                        ; Base (high, bits 24-31)
global GDT64_Data
GDT64_Data equ $ - GDT64
    dw 0xFFFF                                   ; Limit (low, bits 0-15)
    dw 0                                        ; Base (low, bits 0-15)
    db 0                                        ; Base (mid, bits 16-23)
    db PRESENT | NOT_SYS | RW                   ; Access
    db GRANULARITY_4K | SZ_32 | 0xF             ; Flags & Limit (high, bits 16-19)
    db 0                                        ; Base (high, bits 24-31)
global GDT64_TSS
GDT64_TSS equ $ - GDT64
    dw 0xFFFF                                   ; Limit (low, bits 0-15)
    dw 0                                        ; Base (low, bits 0-15)
    db 0                                        ; Base (mid-low, bits 16-23)
    db PRESENT | SYS_TSS_AVL                    ; Access
    db GRANULARITY_4K | 0xF                     ; Flags & Limit (high, bits 16-19)
    db 0                                        ; Base (mid-high, 24-31)
    dd 0                                        ; Base (high, 63-32)
    dd 0                                        ; Reserved
.end:

align 4096
global PML4_table
PML4_table:
; insert flags byte 00000111b
dd 0, 0, 0, 0
.end:

align 4096
global PDPTE_table
PDPTE_table:
dd 0, 0, 0, 0
.end:

align 4096
global PDE_table
PDE_table:
dd 110000111b, 0, 0, 0
dd 110000111b, 0, 0, 0
.end: