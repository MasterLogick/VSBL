#include "IDT.h"
#include <Attributes.h>

#define IDT_DESCRIPTORS_COUNT 36


IDTDescriptor table[IDT_DESCRIPTORS_COUNT] ALIGNED(16);

void (*handlers[IDT_DESCRIPTORS_COUNT])(void) = {
        //                                    // From Table 6-1, “Protected-Mode Exceptions and Interrupts" in the Intel® 64 and IA-32 Architectures Software Developer’s Manual, Volume 3A
        //                                    // +--------+----------+--------------------------------------------+------------+------------+---------------------------------------------------
        //                                    // | Vector | Mnemonic | Description                                | Type       | Error code | Source
        //-----------System interrupts--------// +--------+----------+--------------------------------------------+------------+------------+---------------------------------------------------
        nullptr,                              // |    0   |   #DE    | Divide Error                               | Fault      | No         | No DIV and IDIV instructions.
        nullptr,                              // |    1   |   #DB    | Debug Exception                            | Fault/Trap | No         | Instruction, data, and I/O breakpoints; single-step; and others.
        nullptr,                              // |    2   |   —      | NMI Interrupt                              | Interrupt  | No         | Nonmaskable external interrupt.
        _idt_int_3_handler_asm,               // |    3   |   #BP    | Breakpoint                                 | Trap       | No         | INT3 instruction.
        nullptr,                              // |    4   |   #OF    | Overflow                                   | Trap       | No         | INTO instruction.
        nullptr,                              // |    5   |   #BR    | BOUND Range Exceeded                       | Fault      | No         | BOUND instruction.
        _idt_int_invalid_opcode_asm,          // |    6   |   #UD    | Invalid Opcode (Undefined Opcode)          | Fault      | No         | UD instruction or reserved opcode.
        nullptr,                              // |    7   |   #NM    | Device Not Available (No Math Coprocessor) | Fault      | No         | Floating-point or WAIT/FWAIT instruction.
        _idt_int_DF_handler_asm,              // |    8   |   #DF    | Double Fault                               | Abort      | Yes (zero) | Any instruction that can generate an exception, an NMI, or an INTR.
        nullptr,                              // |    9   |   —      | Coprocessor Segment Overrun (reserved)     | Fault      | No         | Floating-point instruction.
        nullptr,                              // |   10   |   #TS    | Invalid TSS                                | Fault      | Yes        | Task switch or TSS access.
        nullptr,                              // |   11   |   #NP    | Segment Not Present                        | Fault      | Yes        | Loading segment registers or accessing system segments.
        nullptr,                              // |   12   |   #SS    | Stack-Segment                              | Fault      | Yes        | Stack operations and SS register loads.
        nullptr,                              // |   13   |   #GP    | General Protection                         | Fault      | Yes        | Any memory reference and other protection checks.
        _idt_int_page_fault_asm,              // |   14   |   #PF    | Page Fault                                 | Fault      | Yes        | Any memory reference.
        nullptr,                              // |   15   |   —      | (Intel reserved. Do not use.)              | —          | No         | —
        nullptr,                              // |   16   |   #MF    | x87 FPU Floating-Point Error (Math Fault)  | Fault      | No         | x87 FPU floating-point or WAIT/FWAIT instruction.
        nullptr,                              // |   17   |   #AC    | Alignment Check                            | Fault      | Yes (zero) | Any data reference in memory.
        nullptr,                              // |   18   |   #MC    | Machine Check                              | Abort      | No         | Error codes (if any) and source are model dependent.
        nullptr,                              // |   19   |   #XM    | SIMD Floating-Point Exception              | Fault      | No         | SSE/SSE2/SSE3 floating-point instructions.
        nullptr,                              // |   20   |   #VE    | Virtualization Exception                   | Fault      | No         | EPT violations.
        nullptr,                              // |   21   |   #CP    | Control Protection Exception               | Fault      | Yes        | RET, IRET, RSTORSSP, and SETSSBSY instructions can generate this exception. When CET indirect branch tracking is enabled, this exception can be generated due to a missing ENDBRANCH instruction at target of an indirect call or jump.
        //---------Reserved interrupts--------// |  22-31 |   —      | Intel reserved. Do not use                 | —          | —          | —
        nullptr,                              // |   22   |          |                                            |            |            |
        nullptr,                              // |   23   |          |                                            |            |            |
        nullptr,                              // |   24   |          |                                            |            |            |
        nullptr,                              // |   25   |          |                                            |            |            |
        nullptr,                              // |   26   |          |                                            |            |            |
        nullptr,                              // |   27   |          |                                            |            |            |
        nullptr,                              // |   28   |          |                                            |            |            |
        nullptr,                              // |   29   |          |                                            |            |            |
        nullptr,                              // |   30   |          |                                            |            |            |
        nullptr,                              // |   31   |          |                                            |            |            |
        //------------VSBL Interrupts---------// | 32-255 |   —      | User Defined (Non-reserved) Interrupts     | Interrupt  | —          | External interrupt or INT n instruction.
        _idt_int_apic_timer_handler_asm,      // |   32   |   #TM    | APIC timer interrupt                       |            |            |
        nullptr,                              // |   33   |   #SP    | APIC spurious interrupt                    |            |            |
        _idt_int_apic_error_handler_asm,      // |   34   |   #ER    | APIC error interrupt                       |            |            |
        _idt_int_keyboard_handler_asm         // |   35   |   #KB    | Keyboard interrupt                         |            |            |

};

IDTDescriptor::IDTDescriptor(void (*handler)()) : entry{
        static_cast<uint16_t>(reinterpret_cast<uint64_t>(handler) & 0xffff),
        0x8,
        0b1'00'0'1110'0000'0000,
        static_cast<uint16_t>((reinterpret_cast<uint64_t>(handler) >> 16) & 0xffff),
        static_cast<uint16_t>((reinterpret_cast<uint64_t>(handler) >> 32) & 0xffff),
        static_cast<uint16_t>((reinterpret_cast<uint64_t>(handler) >> 48) & 0xffff),
        0,
        0
} {
}

IDTDescriptor::IDTDescriptor() {}

void initIDT() {
    for (int i = 0; i < IDT_DESCRIPTORS_COUNT; ++i) {
        if (handlers[i] != nullptr) {
            table[i] = IDTDescriptor(handlers[i]);
        } else {
            table[i] = IDTDescriptor(_idt_int_fallback_handler_asm);
        }
    }
    _idt_load_table_asm(sizeof(table), table);
    _idt_enable_hardware_interrupts_asm();
}
