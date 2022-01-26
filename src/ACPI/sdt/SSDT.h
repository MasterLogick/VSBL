#ifndef SSDT_H
#define SSDT_H

#include <stdint.h>
#include "DescriptionHeader.h"

struct SSDT : public DescriptionHeader {
    uint32_t creatorID;
    uint32_t creatorRevision;

};


#endif //SSDT_H
