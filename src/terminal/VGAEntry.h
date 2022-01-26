#ifndef VGAENTRY_H
#define VGAENTRY_H

#include "../Attributes.h"
#include <stdint.h>
#include "VGAColor.h"

struct VGAEntry {
    uint8_t ch;
    VGAColor color;
} PACKED;

#endif //VGAENTRY_H
