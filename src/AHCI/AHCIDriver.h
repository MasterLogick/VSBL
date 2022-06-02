#ifndef AHCIDRIVER_H
#define AHCIDRIVER_H

#include "../PCI/PCIDeviceConfigSpace.h"
#include "HBAMemoryRegister.h"
#include "../Vector.h"

class AHCIDriver {
private:
    PCIDeviceConfigSpace deviceConfigSpace;
    HBAMemoryRegister *hbaMemoryRegister;
    Vector<int> availablePorts;

    CommandHeader *getFreeHeader();

public:
    explicit AHCIDriver(PCIDeviceConfigSpace device);
};


#endif //AHCIDRIVER_H
