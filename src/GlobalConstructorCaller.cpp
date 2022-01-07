#include <stdint.h>

typedef void (*GlobalInitializer)();

extern "C" {
GlobalInitializer GlobalInitArray[INT16_MAX];
}

void CallGlobalConstructors() {
    for (int i = 0; GlobalInitArray[i]; ++i) {
        GlobalInitArray[i]();
    }
}