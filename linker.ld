ENTRY(_kernel_entry_asm)
SECTIONS{
	. = 0x7c00;
	Kernel_Start = .;
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

	.init_array : {
	    GlobalInitArray = .;
	    *(.init_array)
	    . = . + 16;
	}

	.bss : {
		*(COMMON)
		*(.bss)
	}
	. = . + 16*1024;
	Kernel_End = .;
	stack_top = .;

	kernel_sectors_count = (SIZEOF(.secondary_bootsection) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.init_array) + SIZEOF(.bss) + 512 - 1) / 512 - 1;
}