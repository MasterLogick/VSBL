#include "VirtualMemoryManager.h"

void *operator new(size_t s) {
    return GlobalVMM->malloc(s);
}

void *operator new[](size_t s) {
    return GlobalVMM->malloc(s);
}

void operator delete(void *ptr) {
    GlobalVMM->free(ptr);
}

void operator delete(void *ptr, size_t s) {
    (void) s;
    GlobalVMM->free(ptr);
}