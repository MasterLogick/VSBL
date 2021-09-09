ENTRY(_kernel_entry_asm)
SECTIONS{
	. = 0x7c00;
	.primary_bootsection BLOCK(512) : ALIGN(512) {
	    *(.primary_bootsection)
	}
	.secondary_bootsection : {
	    *(.secondary_bootsection)
	}
	.text : {
	    *(.text)
	}
	.rodata : {
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data : {
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss : {
		*(COMMON)
		*(.bss)
		*(.stack)
		stack_top = .;
	}
	kernel_sectors_count = (SIZEOF(.secondary_bootsection) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.bss) + 512 - 1) / 512 - 1;
}