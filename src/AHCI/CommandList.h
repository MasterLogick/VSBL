#ifndef COMMANDLIST_H
#define COMMANDLIST_H

#include "CommandHeader.h"

#define COMMAND_HEADERS_COUNT 32

struct CommandList {
    CommandHeader headers[COMMAND_HEADERS_COUNT];

    CommandList();
};


#endif //COMMANDLIST_H
