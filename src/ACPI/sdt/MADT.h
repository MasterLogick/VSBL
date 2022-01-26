#ifndef MADT_H
#define MADT_H

#include <stdint.h>
#include "../../Attributes.h"
#include "DescriptionHeader.h"
#include "ics/ICSHeader.h"

struct MADT : public DescriptionHeader {
    uint32_t apic;
    uint32_t flags;
    ICSHeader ics[0];

    void parse();
} PACKED;

extern MADT *GlobalMADT;

#endif //MADT_H
