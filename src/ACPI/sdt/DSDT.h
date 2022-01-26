#ifndef DSDT_H
#define DSDT_H

#include <stdint.h>
#include "DescriptionHeader.h"

struct DSDT : public DescriptionHeader {
    uint32_t creatorID;
    uint32_t creatorRevision;
    uint8_t amlStream[0];

    void loadBlock();
};


#endif //DSDT_H
