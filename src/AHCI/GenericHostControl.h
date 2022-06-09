#ifndef GENERICHOSTCONTROL_H
#define GENERICHOSTCONTROL_H

#include <stdint.h>

struct GenericHostControl {
    uint32_t hostCaps;
    uint32_t globalHostControl;
    uint32_t interruptStatus;
    uint32_t portsImplemented;
    struct Version {
        uint8_t superMajor;
        uint8_t major;
        uint8_t minor;
        uint8_t subMinor;
    } version;
    uint32_t commandCompletionCoalescingControl;
    uint32_t commandCompletionCoalescingPorts;
    uint32_t enclosureManagementLocation;
    uint32_t enclosureManagementControl;
    uint32_t hostCapabilitiesExtended;
    uint32_t biosHandoffControlStatus;

    bool is64AddressCapable();
};

static_assert(sizeof(GenericHostControl) == 44, "Generic host control structure must be packed");


#endif //GENERICHOSTCONTROL_H
