ENTRY(_real_mode_entry_asm)
SECTIONS{
	. = 0x7c00;
	Kernel_Start = .;
	.real_mode_section : {
	    *(.real_mode_section)
	}

	.real_mode_data : {
	    *(.real_mode_data)
	}

	.protected_mode_section : {
	    *(.protected_mode_section)
	}

	.text : {
	    *(.text)
	}

	.rodata : {
		*(.rodata)
	}
 
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
	.stack ALIGN(16):{
	. = . + 16*1024;
	stack_top = .;
	}
	Kernel_End = .;

	kernel_sectors_count = (SIZEOF(.real_mode_data) + SIZEOF(.protected_mode_section) + SIZEOF(.text) + SIZEOF(.rodata) + SIZEOF(.data) + SIZEOF(.init_array) + 512 - 1) / 512;
}