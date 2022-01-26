#ifndef ICSHEADER_H
#define ICSHEADER_H

#include <stdint.h>
#include "../../../Attributes.h"

struct ICSHeader {
    uint8_t type;
    uint8_t length;
} PACKED;

#endif //ICSHEADER_H
