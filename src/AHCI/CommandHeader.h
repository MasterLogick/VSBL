#ifndef COMMANDHEADER_H
#define COMMANDHEADER_H

#include "stdint.h"
#include "CommandTable.h"

struct CommandHeader {
    uint8_t fisDWLength: 5;
    bool atapi: 1;
    bool write: 1;
    bool prefetchable: 1;
    bool reset: 1;
    bool bist: 1;
    bool clearBusyIfOk: 1;
    bool _RESERVED0: 1;
    uint8_t portMultiplier: 4;
    uint16_t descriptorTableLength;
    uint32_t bytesTransferedCount;
    CommandTable *descriptor;
};


#endif //COMMANDHEADER_H
