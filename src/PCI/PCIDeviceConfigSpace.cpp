#include "PCIDeviceConfigSpace.h"
#include <iostream.h>
#include "PCIConfigSpaceRegister.h"

PCIConfigSpaceRegister<uint16_t> PCIDeviceConfigSpace::getVendorID() {
    return {this, 0};
}

PCIConfigSpaceRegister<uint16_t> PCIDeviceConfigSpace::getDeviceID() {
    return {this, 2};
}

PCIConfigSpaceRegister<uint16_t> PCIDeviceConfigSpace::getCommand() {
    return {this, 4};
}

PCIConfigSpaceRegister<uint16_t> PCIDeviceConfigSpace::getStatus() {
    return {this, 6};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getRevisionID() {
    return {this, 8};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getInterface() {
    return {this, 9};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getSubclass() {
    return {this, 10};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getBaseClassCode() {
    return {this, 11};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getCacheLineSize() {
    return {this, 12};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getLatencyTimer() {
    return {this, 13};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getHeaderType() {
    return {this, 14};
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getBIST() {
    return {this, 15};
}

PCIConfigSpaceRegister<uint32_t> PCIDeviceConfigSpace::getBAR(int i) {
    return {this, static_cast<uint8_t>(0x10 + i * 4)};
}

bool PCIDeviceConfigSpace::exists() {
    return getVendorID()() != 0xffff;
}

void PCIDeviceConfigSpace::printInfo() {
    cout << "PCI: device found: " << bus
         << ":" << device
         << ":" << function
         << ", class: " << getBaseClassCode()()
         << ", subclass: " << getSubclass()()
         << ", interface: " << getInterface()()
         << "\n     header: ";
    uint8_t headerType = getHeaderType()();
    if (headerType & 0x80) {
        cout << "MULT ";
    }
    switch (headerType & 0x7f) {
        case 0:
            cout << "device ";
            break;
        case 1:
            cout << "PCI bridge ";
            break;
        case 2:
            cout << "CardBus bridge ";
            break;
    }
    cout << "\n     command: ";
    uint8_t command = getCommand()();
    if (command & 1) {
        cout << "IO ";
    }
    if (command & 2) {
        cout << "MEM ";
    }
    if (command & 4) {
        cout << "BM ";
    }
    cout << "\n";
}

PCIConfigSpaceRegister<uint8_t> PCIDeviceConfigSpace::getCapabilitiesPtr() {
    return {this, 52};
}
