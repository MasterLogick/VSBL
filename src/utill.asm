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
