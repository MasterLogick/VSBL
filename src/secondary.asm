;Secondary bootloader errors:
;C - CPUID command is unavailable
;E - CPUID extended function limit
;X - x64 architecture is not supported

BITS 32
section .protected_mode_section

extern stack_top
extern _Z5kmainv

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
    jmp _Z5kmainv

.err:
    mov [0xb8000], al
    jmp $
.end:
size _protected_mode_entry_asm _protected_mode_entry_asm.end - _protected_mode_entry_asm
section .data
global PML4_table
align 4096
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
.end: