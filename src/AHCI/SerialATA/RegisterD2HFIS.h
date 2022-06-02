#ifndef REGISTERD2HFIS_H
#define REGISTERD2HFIS_H

#include "stdint.h"
#include "FisType.h"

struct RegisterD2HFIS {
    FisType fitType;
    uint8_t portMultiplier: 4;
    uint8_t _RESERVED0: 2;
    uint8_t interrupt: 1;
    uint8_t _RESERVED1: 1;
    uint8_t status;
    uint8_t error;
    uint8_t lbaLow;
    uint8_t lbaMid;
    uint8_t lbaHigh;
    uint8_t device;
    uint8_t lbaLowExp;
    uint8_t lbaMidExp;
    uint8_t lbaHighExp;
    uint8_t _RESERVED2;
    uint16_t sectCount;
    uint8_t _RESERVED3[6];
};
static_assert(sizeof(RegisterD2HFIS) == 5 * 4, "D2H FIS memory structure must be packed");


#endif //REGISTERD2HFIS_H
