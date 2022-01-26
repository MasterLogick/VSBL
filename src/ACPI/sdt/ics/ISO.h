#ifndef ISO_H
#define ISO_H

#include <stdint.h>
#include "../../../Attributes.h"
#include "ICSHeader.h"

struct ISO : public ICSHeader {
    uint8_t bus;
    uint8_t source;
    uint32_t globalSystemInterrupt;
    uint16_t flags;
} PACKED;

#endif //ISO_H
