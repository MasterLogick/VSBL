#ifndef GENERICADDRESSSTRUCTURE_H
#define GENERICADDRESSSTRUCTURE_H

#include "../Attributes.h"
#include <stdint.h>

struct GenericAddressStructure {
    enum AddressSpaceID {
        SystemMemory = 0,
        SystemIO = 1,
        PCIConfig = 2,
        EmbeddedController = 3,
        SMBus = 4,
        SystemCMOS = 5,
        PciBarTarget = 6,
        IPMI = 7,
        GeneralPurposeIO = 8,
        GenericSerialBus = 9,
        PCC = 10,
        FunctionalFixedHardware = 0x7f
    } PACKED spaceID;
    uint8_t registerBitWidth;
    uint8_t registerBitOffset;
    enum AccessSize {
        undefined = 0,
        byte = 1,
        word = 2,
        dword = 3,
        qword = 4
    } PACKED accessSize;
    void *ptr;
}PACKED;
static_assert(sizeof(GenericAddressStructure) == 12, "GenericAddressStructure must be packed");
#endif //GENERICADDRESSSTRUCTURE_H
