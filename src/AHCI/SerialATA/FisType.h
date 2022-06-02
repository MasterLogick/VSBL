#ifndef FISTYPE_H
#define FISTYPE_H

#include "../../Attributes.h"

enum FisType {
    RegisterH2D = 0x27,
    RegisterD2H = 0x34,
    DMAActivateD2H = 0x39,
    DMASetup = 0x41,
    Data = 0x46,
    BISTActivate = 0x58,
    SetDeviceBitsD2H = 0x5f
} PACKED;
#endif //FISTYPE_H
