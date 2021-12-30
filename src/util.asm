BITS 32
section .text

; char _read_port_asm(unsigned uint16_t port)
global _read_port_asm
_read_port_asm:
    push ebp
    mov ebp, esp
    xor eax, eax
    mov byte [ebp+0x4], al
    ; in al, dx
    ret
.end:
size _read_port_asm _read_port_asm.end - _read_port_asm

; void _int3_asm(void)
global _int3_asm
_int3_asm:
    int 3
    ret
.end:
size _int3_asm _int3_asm.end - _int3_asm

; uint32_t _get_time_asm(void)
global _get_time_asm
_get_time_asm:
    rdtsc
    ret
.end:
size _get_time_asm _get_time_asm.end - _get_time_asm