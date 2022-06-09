#ifndef TERMINAL_H
#define TERMINAL_H

#include <stdint.h>
#include <Vector.h>
#include "VGAEntry.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25

class Terminal {
    int width;
    int height;
    int x;
    int frame;
    int lastUpdateFrame;
    Vector<VGAEntry *> data;
    VGAEntry *const field;
    VGAEntry *currentLine;

    void newLine();

    void updateField();

public:
    void handler(uint8_t scancode, char key, uint32_t event);

    void shiftUp();

    void shiftDown();

    void putChar(VGAEntry entry);

    Terminal();
};

extern Terminal *terminal;

extern void terminalHandler(uint8_t scancode, char key, uint32_t event);

#endif //TERMINAL_H
