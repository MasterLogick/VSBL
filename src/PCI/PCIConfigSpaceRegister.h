#ifndef PCICONFIGSPACEREGISTER_H
#define PCICONFIGSPACEREGISTER_H

#include "stdint.h"
#include "PCI.h"
#include "PCIDeviceConfigSpace.h"

template<typename Var>
class PCIConfigSpaceRegister {
    PCIDeviceConfigSpace *space;
    uint8_t offset;
public:
    PCIConfigSpaceRegister(PCIDeviceConfigSpace *device, uint8_t offset) : space(device),
                                                                           offset(offset) {}

    static_assert(sizeof(Var) <= 4, "Variable size must not exceed four bytes long.");

    PCIConfigSpaceRegister &operator=(Var val) {
        uint32_t line = GlobalPCI->readConfigDataRegister(space->bus, space->device, space->function, offset);
        line = (line & ~(((1 << (sizeof(Var) * 8)) - 1) << ((offset & 3) * 8))) | (val << (offset & 3) * 8);
        GlobalPCI->writeConfigDataRegister(space->bus, space->device, space->function, offset, line);
        return this;
    }

    Var operator()() {
        return GlobalPCI->readConfigDataRegister(space->bus, space->device, space->function, offset)
                >> ((offset & 3) * 8);
    }
};


#endif //PCICONFIGSPACEREGISTER_H
