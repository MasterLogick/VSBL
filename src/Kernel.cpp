#include "iostream.h"
#include "APIC.h"
#include "IDT.h"
#include "ACPI/ACPI.h"
#include "IOAPIC.h"
#include "Keyboard.h"
#include "ps2.h"
#include "PhysicalMemoryManager.h"
#include "GlobalConstructorCaller.h"
#include "Attributes.h"
#include "VirtualMemoryManager.h"

void local_k_handler(uint8_t scancode, char key, uint32_t event) {
    if (event == KEYBOARD_KEY_PRESSED || event == KEYBOARD_KEY_REPEAT) {
        cout << key;
    }
    (void) scancode;
    (void) event;
}

uint8_t vmm[sizeof(VirtualMemoryManager)];

NORETURN void kmain() {
    initIDT();
    GlobalPMM.extractConventionalBlocks();
    GlobalVMM = reinterpret_cast<VirtualMemoryManager *>(vmm);
    GlobalVMM = new(vmm)VirtualMemoryManager();
    CallGlobalConstructors();
    APIC *apic = APIC::getAPIC();
    apic->enableSpuriousInterrupts();
    apic->initLVT();
    apic->setTimerDivider(DIV_128);
//    apic_alarm_timer(apic_base, 0xffffff);
    uint32_t apic_version = apic->version();
    cout << "APIC: " << (isAPICEnabled() ? "enabled" : "disabled") << "\n"
         << "APIC: This processor is " << (isAPICBSP() ? "BSP" : "AP") << "\n"
         << "APIC: base: " << cout.HEX << cout.full << apic << cout.defaults << "\n"
         << "APIC: version: " << cout.HEX << (apic_version & 0xff) << cout.defaults
         << "\n" << "APIC: # of LVT entries: " << (((apic_version >> 16) & 0xff) + 1) << "\n"
         << "APIC: errors: " << apic->errorStatus() << "\n";
    parseACPITables();
    if (!ps2_init()) {
        cout << "PS/2: initialisation failed\n";
    }
    GlobalIOAPIC->redirectIRQ(1, apic, 35);
    cout << "IO APIC: redirection entry count: " << GlobalIOAPIC->getRedirectionEntryCount() << "\n";
    KeyboardSetLocalEventHandler(local_k_handler);
    cout << "APIC: RSDP version " << GlobalRSDP->revision << "\n";
//    terminal_printf("CPUID: max extended value: %x\n", _get_cpuid_leaf_asm(80000008, 0, CPUID_EAX) & 0xff);
    while (true);
}