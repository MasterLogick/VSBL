#include "VirtualMemoryManager.h"

void *operator new(size_t s) {
    return GlobalVMM->malloc(s);
}

void *operator new[](size_t s) {
    return GlobalVMM->malloc(s);
}