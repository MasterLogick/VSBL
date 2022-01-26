#ifndef VGACOLOR_H
#define VGACOLOR_H

#include <stdint.h>
#include "../Attributes.h"

enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GREY = 7,
    DARK_GREY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
} PACKED;

struct VGAColor {
    Color foreground: 4, background: 4;
} PACKED;

static_assert(sizeof(VGAColor) == 1, "VGAColor must be packed");

#endif //VGACOLOR_H
