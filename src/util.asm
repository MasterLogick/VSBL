BITS 64
section .text

; void _int3_asm()
global _int3_asm
_int3_asm:
    int 3
    ret
.end:
size _int3_asm _int3_asm.end - _int3_asm

; uint32_t _get_time_asm()
global _get_time_asm
_get_time_asm:
    rdtsc
    ret
.end:
size _get_time_asm _get_time_asm.end - _get_time_asm

; void _hlt_loop()
global _hlt_loop
_hlt_loop:
    hlt
    jmp _hlt_loop
.end:
size _hlt_loop _hlt_loop.end - _hlt_loop