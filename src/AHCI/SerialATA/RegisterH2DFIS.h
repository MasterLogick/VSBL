#ifndef REGISTERH2DFIS_H
#define REGISTERH2DFIS_H

#include "stdint.h"
#include "FisType.h"

struct RegisterH2DFIS {
    FisType fisType;
    uint8_t pmPort: 4;
    uint8_t _RESERVED0: 3;
    uint8_t c: 1;
    uint8_t command;
    uint8_t features;
    uint8_t lbaLow;
    uint8_t lbaMid;
    uint8_t lbaHigh;
    uint8_t device;
    uint8_t lbaLowExp;
    uint8_t lbaMidExp;
    uint8_t lbaHighExp;
    uint8_t featuresExp;
    uint16_t sectCount;
    uint8_t _RESERVED1;
    uint8_t control;
    uint8_t _RESERVED2[4];
};


#endif //REGISTERH2DFIS_H
