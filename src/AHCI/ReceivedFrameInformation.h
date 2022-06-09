#ifndef RECEIVEDFRAMEINFORMATION_H
#define RECEIVEDFRAMEINFORMATION_H

#include <stdint.h>
#include "SerialATA/RegisterD2HFIS.h"

struct ReceivedFrameInformation {
    uint8_t dmaSetup[0x1c];
    uint32_t _PADDING0;
    uint8_t pioSetup[0x34 - 0x20];
    uint32_t _PADDING1[3];
    RegisterD2HFIS D2HRegister;
    uint32_t _PADDING2;
    uint64_t setDeviceBits;
    uint8_t unknown[0xa0 - 0x60];
    uint8_t _PADDING3[0x60];
};
static_assert(sizeof(ReceivedFrameInformation) == 0x100, "FIS memory structure must be packed");


#endif //RECEIVEDFRAMEINFORMATION_H
