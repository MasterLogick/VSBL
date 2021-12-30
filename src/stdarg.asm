BITS 32
section .text

; void _pr_start_asm(pr_list *list, void *last_arg);
global _pr_start_asm
_pr_start_asm:
    mov eax, [esp + 0x8]
    add eax, 4
    mov ebx, [esp + 0x4]
    mov [ebx], eax
    ret
.end:
size _pr_start_asm _pr_start_asm.end - _pr_start_asm

; uint32_t _pr_arg_asm(pr_list list, int i)
global _pr_arg_asm
_pr_arg_asm:
    mov eax, [esp + 0x4]
    mov ebx, [esp + 0x8]
    mov eax, [eax + 4 * ebx]
    ret
.end:
size _pr_arg_asm _pr_arg_asm.end - _pr_arg_asm