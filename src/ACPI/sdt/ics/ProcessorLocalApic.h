#ifndef PROCESSORLOCALAPIC_H
#define PROCESSORLOCALAPIC_H

#include <stdint.h>
#include "../../../Attributes.h"
#include "ICSHeader.h"

struct ProcessorLocalApic : public ICSHeader {
    uint8_t acpiProcessorID;
    uint8_t apicID;
    uint32_t flags;
} PACKED;

#endif //PROCESSORLOCALAPIC_H
