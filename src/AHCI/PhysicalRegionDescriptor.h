#ifndef PHYSICALREGIONDESCRIPTOR_H
#define PHYSICALREGIONDESCRIPTOR_H

#include <stdint.h>

struct PhysicalRegionDescriptor {
    void *dataBase;
    uint32_t _RESERVED0;
    uint32_t byteCount: 22;
    uint32_t _RESERVED1: 9;
    bool intOnCompletion: 1;
};
static_assert(sizeof(PhysicalRegionDescriptor) == 4 * 4, "Physical region descriptor structure must be packed");

#endif //PHYSICALREGIONDESCRIPTOR_H
