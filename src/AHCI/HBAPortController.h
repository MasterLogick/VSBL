#ifndef HBAPORTCONTROLLER_H
#define HBAPORTCONTROLLER_H

#include "PortControlRegister.h"
#include "Command.h"
#include "../Vector.h"
#include "CommandHolder.h"

class HBAPortController {
    PortControlRegister *port;
    Vector<CommandHolder *> commandHolders;

    CommandHolder *getFreeCommand();

    void enable();

    void disable();

public:

    HBAPortController(PortControlRegister *port);

    bool execute(Command *command);

    void init();
};


#endif //HBAPORTCONTROLLER_H
