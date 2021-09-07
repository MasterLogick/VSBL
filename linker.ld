ENTRY(_kernel_entry_asm)
SECTIONS{
	. = 0x7c00;
	.bootsection BLOCK(512) : ALIGN(512) {
	    *(.bootsection)
	}
	.text BLOCK(512) : ALIGN(512) {
	    *(.text)
	}
	.rodata BLOCK(512) : ALIGN(512) {
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data BLOCK(512) : ALIGN(512) {
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(512) : ALIGN(512) {
		*(COMMON)
		*(.bss)
		*(.stack)
		stack_top = .;
	}
	kernel_sectors_count = (SIZEOF(.bootsection) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.bss) + 512 - 1) / 512 - 1;
}