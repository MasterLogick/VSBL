BITS 64
section .text

; void _io_write8_port_asm(uint16_t port, uint8_t data)
global _io_write8_port_asm
_io_write8_port_asm:
    mov ax, si
    mov dx, di
    out dx, al
    ret
.end:
size _io_write8_port_asm _io_write8_port_asm.end - _io_write8_port_asm

; void _io_write16_port_asm(uint16_t port, uint16_t data)
global _io_write16_port_asm
_io_write16_port_asm:
    mov ax, si
    mov dx, di
    out dx, ax
    ret
.end:
size _io_write16_port_asm _io_write16_port_asm.end - _io_write16_port_asm

; void _io_write32_port_asm(uint16_t port, uint32_t data)
global _io_write32_port_asm
_io_write32_port_asm:
    mov eax, esi
    mov dx, di
    out dx, eax
    ret
.end:
size _io_write32_port_asm _io_write32_port_asm.end - _io_write32_port_asm

; uint8_t _io_read8_port_asm(uint16_t port)
global _io_read8_port_asm
_io_read8_port_asm:
    mov dx, di
    xor rax, rax
    in al, dx
    ret
.end:
size _io_read8_port_asm _io_read8_port_asm.end - _io_read8_port_asm

; uint16_t _io_read16_port_asm(uint16_t port)
global _io_read16_port_asm
_io_read16_port_asm:
    mov dx, di
    xor rax, rax
    in ax, dx
    ret
.end:
size _io_read16_port_asm _io_read16_port_asm.end - _io_read16_port_asm

; uint32_t _io_read32_port_asm(uint16_t port)
global _io_read32_port_asm
_io_read32_port_asm:
    mov dx, di
    xor rax, rax
    in eax, dx
    ret
.end:
size _io_read32_port_asm _io_read32_port_asm.end - _io_read32_port_asm
