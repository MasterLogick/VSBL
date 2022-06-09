#include "PCI.h"
#include <IO.h>
#include <AHCI/AHCIDriver.h>
#include <network/AM79C970A/AM79C970ADriver.h>
#include "PCIDeviceConfigSpace.h"
#include "PCIConfigSpaceRegister.h"

#define CONFIG_ADDRESS 0xCF8
#define CONFIG_DATA 0xCFC

uint32_t PCI::readConfigDataRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset) {
    _io_write32_port_asm(CONFIG_ADDRESS, (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xfc));
    return _io_read32_port_asm(CONFIG_DATA);
}

void PCI::writeConfigDataRegister(uint8_t bus, uint8_t device, uint8_t function, uint8_t offset, uint32_t data) {
    _io_write32_port_asm(CONFIG_ADDRESS, (1 << 31) | (bus << 16) | (device << 11) | (function << 8) | (offset & 0xfc));
    _io_write32_port_asm(CONFIG_DATA, data);
}

void PCI::enumerateFunctions() {
    for (int bus = 0; bus < 256; ++bus) {
        for (int device = 0; device < 32; ++device) {
            for (int function = 0; function < 8; ++function) {
                PCIDeviceConfigSpace func{static_cast<uint8_t>(bus), static_cast<uint8_t>(device),
                                          static_cast<uint8_t>(function)};
                if (func.exists()) {
                    func.printInfo();
                    PCIConfigSpaceRegister<uint8_t> baseClass = func.getBaseClassCode();
                    PCIConfigSpaceRegister<uint8_t> subclass = func.getSubclass();
                    PCIConfigSpaceRegister<uint8_t> interface = func.getInterface();
                    if (baseClass() == 1 && subclass() == 6 && interface() == 1) {
                        AHCIDriver driver(func);
                    } else if (baseClass() == 2 && subclass() == 0 && interface() == 0) {
                        AM79C970ADriver driver(func);
                    }
                }
            }
        }
    }
}

PCI *GlobalPCI;