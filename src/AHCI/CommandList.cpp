#include "CommandList.h"

CommandList::CommandList() {
    for (int i = 0; i < COMMAND_HEADERS_COUNT; ++i) {
        headers[i].descriptor = nullptr;
    }
}
