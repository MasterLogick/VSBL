#ifndef DESCRIPTIONHEADER_H
#define DESCRIPTIONHEADER_H

#include <stdint.h>
#include "../../Attributes.h"

struct DescriptionHeader {
    char signature[4];
    uint32_t length;
    uint8_t revision;
    uint8_t checksum;
    char OEMID[6];
    uint64_t OEMTableID;
    uint32_t OEMRevision;
    uint32_t CreatorID;
    uint32_t CreatorRevision;

    bool isValid();
} PACKED;

#endif //DESCRIPTIONHEADER_H
