#ifndef RSDP_H
#define RSDP_H

#include <stdint.h>
#include "../../Attributes.h"
#include "XSDT.h"

struct RSDP {
    char signature[8];
    uint8_t checksum;
    char OEMID[6];
    uint8_t revision;
    uint32_t rsdtPtr;
    uint32_t length;
    XSDT *xsdt;
    uint8_t extendedChecksum;

    bool isValid();

    static RSDP *find();
} PACKED;

extern RSDP *GlobalRSDP;

#endif //RSDP_H
