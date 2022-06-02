#ifndef HBAMEMORYREGISTER_H
#define HBAMEMORYREGISTER_H

#include "stdint.h"
#include "../Attributes.h"
#include "GenericHostControl.h"
#include "PortControlRegister.h"

struct HBAMemoryRegister {
    GenericHostControl genericHostControl;
    uint8_t vendor[0x100 - sizeof(GenericHostControl)];
    PortControlRegister ports[32];
};
static_assert(sizeof(HBAMemoryRegister) == 0x1100, "Host bus adapter memory structure must be packed");
#endif //HBAMEMORYREGISTER_H
