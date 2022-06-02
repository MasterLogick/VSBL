#include "HBAPortController.h"
#include "../terminal/basic_stream.h"
#include "../terminal/basic_stream.h"
#include "Command.h"
#include "../VirtualMemoryManager.h"
#include "SerialATA/RegisterH2DFIS.h"
#include "../util.h"

CommandHolder *HBAPortController::getFreeCommand() {
    for (CommandHolder *&item: commandHolders) {
        if (item->isFree()) return item;
    }
    if (commandHolders.getSize() == COMMAND_HEADERS_COUNT) {
        return nullptr;
    }
    CommandHolder *c = new CommandHolder(port, commandHolders.getSize());
    commandHolders.push_back(c);
    return c;
}

HBAPortController::HBAPortController(PortControlRegister *port) : port(port) {
}

bool HBAPortController::execute(Command *command) {
    CommandHolder *commandHeader = getFreeCommand();
    if (commandHeader == nullptr) {
        cout << "AHCI:HBA is busy" << endl;
        return false;
    }
    cout << "Command header selected: " << commandHeader << endl;
    port->interruptStatus = 0;
    commandHeader->load(command);
    commandHeader->issue();
    commandHeader->awaitCompletion();
    return true;
}

void HBAPortController::init() {
    disable();
    enable();
}

void HBAPortController::enable() {
    while (port->commandAndStatus & (1 << 15));
    port->commandAndStatus |= 1 << 4;
    port->commandAndStatus |= 1;
}

void HBAPortController::disable() {
    port->commandAndStatus &= ~1;
    port->commandAndStatus &= ~(1 << 4);
    while (true) {
        if (port->commandAndStatus & (3 << 14))continue;
        break;
    }
}
