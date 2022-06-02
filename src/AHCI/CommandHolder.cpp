#include "CommandHolder.h"
#include "../VirtualMemoryManager.h"
#include "../string.h"
#include "../terminal/basic_stream.h"

CommandHolder::CommandHolder(PortControlRegister *port, int index) : port(port), index(index) {
    header = port->commandListBaseAddress->headers + index;
    table = header->descriptor;
    this->prdtCount = 1;
}

bool CommandHolder::isFree() {
    cout << "command issue: " << port->commandIssue << endl;
    cout << "sata active: " << port->sataActive << endl;
    return ((port->commandIssue | port->sataActive) & (1 << index)) == 0;
}

void CommandHolder::load(Command *command) {
    header->fisDWLength = command->fisBSize / sizeof(uint32_t);
    header->descriptorTableLength = command->prdtl;
    header->write = command->write;
    memcpy(command->commandFis, table->commandFIS, command->fisBSize);
    memcpy(command->prdt, table->prdt, command->prdtl * sizeof(uint32_t) * 4);
}

void CommandHolder::issue() {
    port->commandIssue = port->commandIssue | (1 << index);
}

bool CommandHolder::isCompleted() {
    if (port->interruptStatus) {
        return true;
    }
    if ((port->commandIssue & (1 << index)) == 0) {
        return true;
    }
    return false;
}

void CommandHolder::awaitCompletion() {
    while (!isCompleted());
}
