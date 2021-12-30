BITS 32
section .text

; void _msr_set_asm(uint32_t reg, uint64_t value)
global _msr_set_asm
_msr_set_asm:
    mov ecx, [esp+0x4]
    mov eax, [esp+0x8]
    mov edx, [esp+0xc]
    wrmsr
    ret
.end:
size _msr_set_asm _msr_set_asm.end - _msr_set_asm

; uint64_t _msr_get_asm(uint32_t reg)
global _msr_get_asm
_msr_get_asm:
    mov ecx, [esp+0x4]
    rdmsr
    ret
.end:
size _msr_get_asm _msr_get_asm.end - _msr_get_asm