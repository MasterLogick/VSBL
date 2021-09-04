BITS 32

extern kernel_text_section_sectors_count
extern stack_top
section .stack
align 16
resb 16*1024 ; 16 KiB

section .bootsection
global _kernel_entry_asm

_kernel_entry_asm:

jmp main

;bx = value to print
dbg_print:
    push ax
    push bx
    push cx
    mov ecx, 16
l_1:
    shl bx, 1
    mov al, '0'
    jnc c_2
    mov al, '1'
c_2:
    call error_print
    loop l_1
    pop cx
    pop bx
    pop ax
    ret


;al = ascii code to print
error_print:
    push ax
    mov ah, 0x0e
    int 0x10
    pop ax
    ret



main:
    cli
    mov esp, stack_top
    sti
    mov al, 'A'
    call error_print
    ;Read drive parameters
    mov ax, 0
    mov es, ax
    mov di, 0
    mov ah, 0x08
    mov dl, drive_number
    int 0x13

    mov al, 'E'
    jnc c_1
    call error_print
    jmp c_1_end
c_1:
    mov al, 'C'
    call error_print
c_1_end:

    movzx bx, ah
    call dbg_print
    mov al, ' '
    call error_print
    
    movzx bx, dl
    call dbg_print
    mov al, ' '
    call error_print
    
    movzx bx, dh
    call dbg_print
    mov al, ' '
    call error_print

    mov bx, cx
    call dbg_print
    mov al, ' '
    call error_print

    ;Extended read
    mov ax, 0
    mov ds, ax
    mov ah, 0x42
    mov dl, drive_number
    mov si, data_packet
    int 0x13
    jc err

    ;extern kmain
    ;call kmain
    
    mov al,'a'
    call error_print
err:
    cli
h:  
    hlt
    jmp h

data_packet:
    db 0x10, 0
    dw 0x1
    dd kernel_text_section_sectors_count
    dq 0x1
drive_number equ 0x80
times 512 - 2 - ($ - $$) db 0
db 0x55
db 0xaa

_kernel_entry_asm_end:
size _kernel_entry_asm _kernel_entry_asm_end-_kernel_entry_asm