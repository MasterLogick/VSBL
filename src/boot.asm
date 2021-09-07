BITS 16

extern kernel_sectors_count
extern stack_top
extern kmain

; section .stack
; align 16
; resb 16*1024 ; 16 KiB

section .bootsection
global _kernel_entry_asm

_kernel_entry_asm:
    cli
    mov esp, stack_top
    sti

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
    mov al, 'c'
    jne .c1
    mov al, 'e'
.c1:
    call error_print

    ;Extended read
    mov ax, 0
    mov ds, ax
    mov ah, 0x42
    mov dl, drive_number
    mov si, data_packet
    int 0x13
    mov al, 'e'
    jc .err
    mov al, 'c'
    call error_print
    call kmain
.err:
    call error_print
    cli
.h:  
    hlt
    jmp .h
.end:
size _kernel_entry_asm _kernel_entry_asm.end - _kernel_entry_asm

;bx = value to print
global dbg_print
dbg_print:
    push ax
    push bx
    push cx
    mov cx, 16
.l_1:
    shl bx, 1
    mov al, '0'
    jnc .c_2
    mov al, '1'
.c_2:
    call error_print
    loop .l_1
    pop cx
    pop bx
    pop ax
    ret
.end:
size dbg_print dbg_print.end - dbg_print

;al = ascii code to print
global error_print
error_print:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret
.end:
size error_print error_print.end - error_print

data_packet:
    db 0x10 ; packet size
    db 0    ; reserved (0)
    dw kernel_sectors_count  ; blocks to read
    dd transfer_buffer_ptr
    dq 0x1
drive_number equ 0x80
times 512 - 2 - ($ - $$) db 0
db 0x55
db 0xaa
transfer_buffer_ptr:
