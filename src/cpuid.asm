BITS 64
section .text

CPUID_EAX_ASM equ 0
CPUID_EBX_ASM equ 1
CPUID_ECX_ASM equ 2
CPUID_EDX_ASM equ 3
global CPUID_EAX_ASM
global CPUID_EBX_ASM
global CPUID_ECX_ASM
global CPUID_EDX_ASM

; uint32_t _get_cpuid_leaf_asm(uint32_t leaf, uint32_t param, uint8_t register_id)
global _get_cpuid_leaf_asm
_get_cpuid_leaf_asm:
    xor rax, rax
    mov eax, edi
    mov ecx, esi
    cpuid
    cmp byte dl, CPUID_EAX_ASM
    jne .not_eax
    jmp .r
.not_eax:
    cmp byte dl, CPUID_EBX_ASM
    jne .not_ebx
    mov eax, ebx
    jmp .r
.not_ebx:
    cmp byte dl, CPUID_ECX_ASM
    jne .not_ecx
    mov eax, ecx
    jmp .r
.not_ecx:
    cmp byte dl, CPUID_EDX_ASM
    mov eax, edx
    jne .r
.r:
    ret
.end:
size _get_cpuid_leaf_asm _get_cpuid_leaf_asm.end - _get_cpuid_leaf_asm
