#ifndef COMMANDHOLDER_H
#define COMMANDHOLDER_H


#include "PortControlRegister.h"
#include "Command.h"

class CommandHolder {
    PortControlRegister *port;
    CommandHeader *header;
    CommandTable *table;
    int index;
    int prdtCount;
public:
    CommandHolder(PortControlRegister *port, int index);

    bool isFree();

    void load(Command *command);

    void issue();

    bool isCompleted();

    void awaitCompletion();
};


#endif //COMMANDHOLDER_H
