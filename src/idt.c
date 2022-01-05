#include "idt.h"
#include <stddef.h>

idt_descriptor idt_table[IDT_DESCRIPTORS_COUNT] __attribute__ ((aligned (16)));

void (*idt_handlers[IDT_DESCRIPTORS_COUNT])(void) = {
        //                                    // From Table 6-1, “Protected-Mode Exceptions and Interrupts" in the Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3A
        //                                    // +--------+----------+--------------------------------------------+------------+------------+---------------------------------------------------
        //                                    // | Vector | Mnemonic | Description                                | Type       | Error code | Source
        //-----------System interrupts--------// +--------+----------+--------------------------------------------+------------+------------+---------------------------------------------------
        NULL,                                 // |    0   |   #DE    | Divide Error                               | Fault      | No         | No DIV and IDIV instructions.
        NULL,                                 // |    1   |   #DB    | Debug Exception                            | Fault/Trap | No         | Instruction, data, and I/O breakpoints; single-step; and others.
        NULL,                                 // |    2   |   —      | NMI Interrupt                              | Interrupt  | No         | Nonmaskable external interrupt.
        _idt_int_3_handler_asm,               // |    3   |   #BP    | Breakpoint                                 | Trap       | No         | INT3 instruction.
        NULL,                                 // |    4   |   #OF    | Overflow                                   | Trap       | No         | INTO instruction.
        NULL,                                 // |    5   |   #BR    | BOUND Range Exceeded                       | Fault      | No         | BOUND instruction.
        NULL,                                 // |    6   |   #UD    | Invalid Opcode (Undefined Opcode)          | Fault      | No         | UD instruction or reserved opcode.
        NULL,                                 // |    7   |   #NM    | Device Not Available (No Math Coprocessor) | Fault      | No         | Floating-point or WAIT/FWAIT instruction.
        _idt_int_DF_handler_asm,              // |    8   |   #DF    | Double Fault                               | Abort      | Yes (zero) | Any instruction that can generate an exception, an NMI, or an INTR.
        NULL,                                 // |    9   |   —      | Coprocessor Segment Overrun (reserved)     | Fault      | No         | Floating-point instruction.
        NULL,                                 // |   10   |   #TS    | Invalid TSS                                | Fault      | Yes        | Task switch or TSS access.
        NULL,                                 // |   11   |   #NP    | Segment Not Present                        | Fault      | Yes        | Loading segment registers or accessing system segments.
        NULL,                                 // |   12   |   #SS    | Stack-Segment                              | Fault      | Yes        | Stack operations and SS register loads.
        NULL,                                 // |   13   |   #GP    | General Protection                         | Fault      | Yes        | Any memory reference and other protection checks.
        NULL,                                 // |   14   |   #PF    | Page Fault                                 | Fault      | Yes        | Any memory reference.
        NULL,                                 // |   15   |   —      | (Intel reserved. Do not use.)              | —          | No         | —
        NULL,                                 // |   16   |   #MF    | x87 FPU Floating-Point Error (Math Fault)  | Fault      | No         | x87 FPU floating-point or WAIT/FWAIT instruction.
        NULL,                                 // |   17   |   #AC    | Alignment Check                            | Fault      | Yes (zero) | Any data reference in memory.
        NULL,                                 // |   18   |   #MC    | Machine Check                              | Abort      | No         | Error codes (if any) and source are model dependent.
        NULL,                                 // |   19   |   #XM    | SIMD Floating-Point Exception              | Fault      | No         | SSE/SSE2/SSE3 floating-point instructions.
        NULL,                                 // |   20   |   #VE    | Virtualization Exception                   | Fault      | No         | EPT violations.
        NULL,                                 // |   21   |   #CP    | Control Protection Exception               | Fault      | Yes        | RET, IRET, RSTORSSP, and SETSSBSY instructions can generate this exception. When CET indirect branch tracking is enabled, this exception can be generated due to a missing ENDBRANCH instruction at target of an indirect call or jump.
        //---------Reserved interrupts--------// |  22-31 |   —      | Intel reserved. Do not use                 | —          | —          | —
        NULL,                                 // |   22   |          |                                            |            |            |
        NULL,                                 // |   23   |          |                                            |            |            |
        NULL,                                 // |   24   |          |                                            |            |            |
        NULL,                                 // |   25   |          |                                            |            |            |
        NULL,                                 // |   26   |          |                                            |            |            |
        NULL,                                 // |   27   |          |                                            |            |            |
        NULL,                                 // |   28   |          |                                            |            |            |
        NULL,                                 // |   29   |          |                                            |            |            |
        NULL,                                 // |   30   |          |                                            |            |            |
        NULL,                                 // |   31   |          |                                            |            |            |
        //------------VSBL Interrupts---------// | 32-255 |   —      | User Defined (Non-reserved) Interrupts     | Interrupt  | —          | External interrupt or INT n instruction.
        _idt_int_apic_timer_handler_asm,      // |   32   |   #TM    | APIC timer interrupt                       |            |            |
        NULL,                                 // |   33   |   #SP    | APIC spurious interrupt                    |            |            |
        _idt_int_apic_error_handler_asm,      // |   34   |   #ER    | APIC error interrupt                       |            |            |
        _idt_int_keyboard_handler_asm         // |   35   |   #KB    | Keyboard interrupt                         |            |            |

};

idt_descriptor idt_create_descriptor(void (*handler_ptr)(void)) {
    idt_descriptor descriptor;
    descriptor.entry[0] = (uint16_t) (uintptr_t) handler_ptr;
    descriptor.entry[1] = 0x8;
    descriptor.entry[2] = 0b1000111000000000;
    descriptor.entry[3] = (uint16_t) ((uint32_t) (uintptr_t) handler_ptr >> 16);
    return descriptor;
}

void idt_init(void) {
    for (int i = 0; i < IDT_DESCRIPTORS_COUNT; ++i) {
        if (idt_handlers[i] != NULL) {
            idt_table[i] = idt_create_descriptor(idt_handlers[i]);
        } else {
            idt_table[i] = idt_create_descriptor(_idt_int_fallback_handler_asm);
        }
    }
    _idt_load_table_asm(IDT_DESCRIPTORS_COUNT * sizeof(idt_descriptor), idt_table);
}
