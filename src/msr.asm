BITS 64
section .text

; void _msr_set_asm(uint32_t reg, uint64_t value)
global _msr_set_asm
_msr_set_asm:
    mov ecx, edi
    mov eax, esi
    mov edx, edx
    wrmsr
    ret
.end:
size _msr_set_asm _msr_set_asm.end - _msr_set_asm

; uint64_t _msr_get_asm(uint32_t reg)
global _msr_get_asm
_msr_get_asm:
    mov ecx, edi
    rdmsr
    xchg eax, edx
    shl rax, 32
    mov eax, edx
    ret
.end:
size _msr_get_asm _msr_get_asm.end - _msr_get_asm