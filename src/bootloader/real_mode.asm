;VSBL - Very Simple Boot Loader

;Primary bootloader errors (last letter in the terminal):
;A - A20 line is still disabled after attempt of loading
;N - no hdd drives found
;D - hard drive sector read error
;K - secondary bootloader code read error
;M - memory map obtaining error

BITS 16

section .real_mode_section

extern kernel_sectors_count
extern _protected_mode_entry_asm

bootloader_start:

global _real_mode_entry_asm
_real_mode_entry_asm:

    ;load temporary stack pointer
    cli
    xor ax, ax
    mov ss, ax
    mov sp, 0x7c00

    ;enable A20 memory bus
    in al, 0x92
    or al, 2
    out 0x92, al

    ;disable PIC
    mov al, 0xff
    out 0xa1, al
    out 0x21, al

    ;check A20 memory bus line enabled
    xor ax, ax
    mov ds, ax
    not ax
    mov es, ax
    mov di, signature
    mov si, signature + 0x100000-0xffff0
    mov word bx, [es:si]
    not bx
    mov eax, [ds:di]
    mov word [ds:di], bx
    mov word cx, [es:si]
    mov [ds:di], eax
    cmp cx, bx
    jne .c1
    mov al, 'A'
    jmp .err

    ;find drive with our kernel
    ;first hdd number is 0x80
.c1:
    mov dl, 0x7f
    mov cl, [0x0475]
    cmp cl, 0
    mov al, 'N'
    je .err
.l:
    inc dl
    xor ax, ax
    mov ds, ax
    mov ah, 0x42
    mov si, test_data_packet
    int 0x13
    mov al, 'D'
    jc .err
    mov eax, [signature]
    mov ebx, [transfer_buffer_ptr + signature - bootloader_start]
    cmp eax, ebx
    je .br 
    loop .l

    ;load kernel from hard drive
.br:
    mov bx, 1-127
    mov cx, kernel_sectors_count
    mov edi, 0x7c00 + 0x200 - 127 * 512
.read_loop:
    add bx, 127
    cmp cx, 127
    jl .read_br_else
    sub cx, 127
    jmp .read_br_end
.read_br_else:
    xor cx, cx
.read_br_end:
    mov [data_packet.lba], bx
    add edi, 127 * 512
    mov [data_packet.ptr_addr], di
    shr edi, 16
    shl edi, 16-4
    mov [data_packet.ptr_segm], di
    shl edi, 8
    mov di, [data_packet.ptr_addr]
    xor ax, ax
    mov ds, ax
    mov ah, 0x42
    mov si, data_packet
    int 0x13
    mov al, 'K'
    jc .err
    test cx, cx
    jnz .read_loop

    ; get memory map
    clc
    xor ebx, ebx
    xor si, si
    xor ax, ax
    mov es, ax
    mov di, _PAM_asm
    mov ecx, 24
    mov eax, 0xE820
    mov edx, 'PAMS'
    mov dword [es:di + 20], dword 1
    int 0x15
    jc .m_err
    cmp eax, 'PAMS'
    jne .m_err
    jmp .m_entry_process
.m_get_entry:
    clc
    mov eax, 0xE820
    mov ecx, 24
    mov edx, 'PAMS'
    mov dword [es:di + 20], dword 1
    int 0x15
    jc .m_end
    test ebx, ebx
    jz .m_end
.m_entry_process:
    jcxz .m_get_entry
    add di, 24
    inc si
    cmp si, 30
    jge .m_err
    jmp .m_get_entry
.m_err:
    mov al, 'M'
    jmp .err
.m_end:
    inc si
    mov [_PAM_asm + 24 * 30], si
    clc

    ;setup GDT
    xor ax, ax
    mov ds, ax
    mov es, ax
    lgdt [GDT_DESCRIPTION]

    ;transfer to protected mode
    mov eax, 0x11
    mov cr0, eax
    jmp 0x08:_protected_mode_entry_asm
.end:
size _real_mode_entry_asm _real_mode_entry_asm.end - _real_mode_entry_asm

;al = ascii code to print
.err:
_error_print_asm:
    mov bx, 0xb000
    mov ds, bx
    mov bx, 0x8000
    mov ah, 7
    mov [ds:bx], ax
    jmp $
.end:

terminal_str: dw 0x8000

test_data_packet:
    db 0x10 ; packet size
    db 0    ; reserved (0)
    dw 1  ; blocks to read
    dd transfer_buffer_ptr
    dq 0x0

data_packet:
    db 0x10 ; packet size
    db 0    ; reserved (0)
    dw 127  ; blocks to read
.ptr_addr:
    dw 0
.ptr_segm:
    dw 0
.lba:
    dd 0,0

times 0x200 - 2 - 4 - ($ - $$) db 0

signature: db "VSBL"
db 0x55
db 0xaa

bootsection_end:
transfer_buffer_ptr:

section .real_mode_data

align 4
GDT_DESCRIPTION:
dw GDT.end - GDT - 1
dd GDT, 0

align 8
GDT:

.gdte_zero:
dd 0, 0

.gdte_code_compat_mode:
; limit 0-15 bits
dw 0xffff
; base 0-15 bits
dw 0x0
; base 16-23 bits
db 0x0
; access byte
db 10011010b
; flags, limit 16-19 bits
db 11001111b
; base 24-31 bits
db 0x0

.gdte_data_compat_mode:
; limit 0-15 bits
dw 0xffff
; base 0-15 bits
dw 0x0
; base 16-23 bits
db 0x0
; access byte
db 10010010b
; flags, limit 16-19 bits
db 11001111b
; base 24-31 bits
db 0x0
.end:

global _PAM_asm
_PAM_asm:
resb 24 * 30 + 8
