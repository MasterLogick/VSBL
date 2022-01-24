BITS 64
section .text

; void _paging_load_PML4Table(PML4Entry *table)
global _paging_load_PML4Table
_paging_load_PML4Table:
    mov cr3, rdi
    ret
.end:
size _paging_load_PML4Table _paging_load_PML4Table.end - _paging_load_PML4Table