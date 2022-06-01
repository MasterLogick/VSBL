typedef void (*GlobalInitializer)();

extern GlobalInitializer GlobalInitArray;

void CallGlobalConstructors() {
    for (int i = 0; (&GlobalInitArray)[i]; ++i) {
        (&GlobalInitArray)[i]();
    }
}