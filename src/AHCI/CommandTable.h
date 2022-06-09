#ifndef COMMANDTABLE_H
#define COMMANDTABLE_H

#include <stdint.h>
#include "PhysicalRegionDescriptor.h"

struct CommandTable {
    uint8_t commandFIS[0x40];
    uint8_t atapiCommand[0x10];
    uint8_t _RESERVED0[0x30];
    PhysicalRegionDescriptor prdt[0];
};


#endif //COMMANDTABLE_H
