#ifndef COMMAND_H
#define COMMAND_H


#include "PhysicalRegionDescriptor.h"

struct Command {
    void *commandFis;
    int fisBSize;
    PhysicalRegionDescriptor *prdt;
    int prdtl;
    bool write;
};


#endif //COMMAND_H
