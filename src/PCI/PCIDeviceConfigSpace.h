#ifndef PCIDEVICE_H
#define PCIDEVICE_H

#include <stdint.h>

template<typename T>
class PCIConfigSpaceRegister;

struct PCIDeviceConfigSpace {
    uint8_t bus;
    uint8_t device;
    uint8_t function;

    PCIConfigSpaceRegister<uint16_t> getVendorID();

    PCIConfigSpaceRegister<uint16_t> getDeviceID();

    PCIConfigSpaceRegister<uint16_t> getCommand();

    PCIConfigSpaceRegister<uint16_t> getStatus();

    PCIConfigSpaceRegister<uint8_t> getRevisionID();

    PCIConfigSpaceRegister<uint8_t> getInterface();

    PCIConfigSpaceRegister<uint8_t> getSubclass();

    PCIConfigSpaceRegister<uint8_t> getBaseClassCode();

    PCIConfigSpaceRegister<uint8_t> getCacheLineSize();

    PCIConfigSpaceRegister<uint8_t> getLatencyTimer();

    PCIConfigSpaceRegister<uint8_t> getHeaderType();

    PCIConfigSpaceRegister<uint8_t> getBIST();

    PCIConfigSpaceRegister<uint32_t> getBAR(int i);

    PCIConfigSpaceRegister<uint8_t> getCapabilitiesPtr();

    bool exists();

    void printInfo();
};


#endif //PCIDEVICE_H
