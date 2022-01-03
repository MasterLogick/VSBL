BITS 32
section .text

; void _io_write8_port_asm(uint16_t port, uint8_t data)
global _io_write8_port_asm
_io_write8_port_asm:
    mov al, byte [esp+0x8]
    mov dx, word [esp+0x4]
    out dx, al
    ret
.end:
size _io_write8_port_asm _io_write8_port_asm.end - _io_write8_port_asm

; void _io_write16_port_asm(uint16_t port, uint16_t data)
global _io_write16_port_asm
_io_write16_port_asm:
    mov ax, word [esp+0x8]
    mov dx, word [esp+0x4]
    out dx, ax
    ret
.end:
size _io_write16_port_asm _io_write16_port_asm.end - _io_write16_port_asm

; void _io_write32_port_asm(uint16_t port, uint32_t data)
global _io_write32_port_asm
_io_write32_port_asm:
    mov eax, dword [esp+0x8]
    mov dx, word [esp+0x4]
    out dx, eax
    ret
.end:
size _io_write32_port_asm _io_write32_port_asm.end - _io_write32_port_asm

; uint8_t _io_read8_port_asm(uint16_t port)
global _io_read8_port_asm
_io_read8_port_asm:
    mov dx, word [esp+0x4]
    xor eax, eax
    in al, dx
    ret
.end:
size _io_read8_port_asm _io_read8_port_asm.end - _io_read8_port_asm

; uint16_t _io_read16_port_asm(uint16_t port)
global _io_read16_port_asm
_io_read16_port_asm:
    mov dx, word [esp+0x4]
    xor eax, eax
    in ax, dx
    ret
.end:
size _io_read16_port_asm _io_read16_port_asm.end - _io_read16_port_asm

; uint32_t _io_read32_port_asm(uint16_t port)
global _io_read32_port_asm
_io_read32_port_asm:
    mov dx, word [esp+0x4]
    xor eax, eax
    in eax, dx
    ret
.end:
size _io_read32_port_asm _io_read32_port_asm.end - _io_read32_port_asm
