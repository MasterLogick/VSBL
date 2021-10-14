;VSBL - Very Simple Boot Loader

;Primary bootloader errors:
;A - A20 line is still disabled after attempt of loading
;N - no hdd drives found
;D - the first hard drive sector read error
;K - secondary bootloader code read error

BITS 16

extern kernel_sectors_count
extern _protected_mode_entry_asm

section .stack
align 16
resb 16*1024 ; 16 KiB

section .primary_bootsection
global _kernel_entry_asm

_kernel_entry_asm:
    ;load temporary stack pointer
    cli
    mov esp, 0x7bff
    sti

    ;enable A20 memory bus
    in al, 0x92
    or al, 2
    out 0x92, al

    ;check A20 memory bus line enabled
    xor ax, ax
    mov ds, ax
    not ax
    mov es, ax
    mov di, 0x7dfe
    mov si, 0x7e0e
    mov word bx, [es:si]
    not bx
    mov word [ds:di], bx
    mov word cx, [es:si]
    cmp cx, bx
    jne .c1
    mov al, 'A'
    jmp .err

.c1:
    ;find drive with our kernel  
    ;first hdd number is 0x80
    mov dl, 0x7f
    mov cl, [0x0475]
    cmp cl, 0
    mov al, 'N'
    jz .err
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
    mov ebx, [0x7c00 + 0x200 - 6]
    cmp eax, ebx
    je .br 
    loop .l
    ;load kernel from hard drive
.br:
    xor ax, ax
    mov ds, ax
    mov ah, 0x42
    mov dl, drive_number
    mov si, data_packet
    int 0x13
    mov al, 'K'
    jc .err

    ;setup GDT
    xor ax, ax
    mov ds, ax
    mov es, ax
    cli
    lgdt [GDT_DESCRIPTION]

    ;transfer to protected mode
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp 0x08:_protected_mode_entry_asm
.err:
    call _error_print_asm
    cli
.h:  
    hlt
    jmp .h
.end:
size _kernel_entry_asm _kernel_entry_asm.end - _kernel_entry_asm

;al = ascii code to print
global _error_print_asm
_error_print_asm:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret
.end:

;al = byte to print in binary format
global _binary_print_asm
_binary_print_asm:
    push ax
    push bx
    push cx
    mov ah, 0x0e
    mov cx, 8
    mov bl, al
.l1:
    shl bl, 1
    mov al, '0'
    jnc .else
    inc al
.else:
    int 0x10
    loop .l1
    pop cx
    pop bx
    pop ax
    ret
.end:
size _binary_print_asm _binary_print_asm.end - _binary_print_asm
size _error_print_asm _error_print_asm.end - _error_print_asm

test_data_packet:
    db 0x10 ; packet size
    db 0    ; reserved (0)
    dw 1  ; blocks to read
    dd transfer_buffer_ptr
    dq 0x0

data_packet:
    db 0x10 ; packet size
    db 0    ; reserved (0)
    dw kernel_sectors_count  ; blocks to read
    dd transfer_buffer_ptr
    dq 0x1
drive_number equ 0x80

align 4
GDT_DESCRIPTION:
dw GDT.end - GDT - 1
dd GDT
dd 0

align 8
GDT:

.gdte_zero:
dq 0

.gdte_code:
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

.gdte_data:
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

times 512 - 6 - ($ - $$) db 0
signature: db "VSBL"
db 0x55
db 0xaa
transfer_buffer_ptr: