#include "terminal.h"
#include "cpuid.h"
#include <stddef.h>

union conv{
    char data[4];
    uint32_t conv;
} conv;
void kmain(void) {
	terminal_initialize();
    //terminal_writestring("Hello from kernel World! Use this power wisely!");
    conv.conv = _get_cpuid_leaf_asm(0,CPUID_EBX);
    terminal_writestring(conv.data);
    conv.conv = _get_cpuid_leaf_asm(0,CPUID_ECX);
    terminal_writestring(conv.data);
    conv.conv = _get_cpuid_leaf_asm(0,CPUID_EDX);
    terminal_writestring(conv.data);
}