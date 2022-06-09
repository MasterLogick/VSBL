#include "Terminal.h"
#include <string.h>
#include <Keyboard.h>
#include <iostream.h>

Terminal *terminal;

void terminalHandler(uint8_t scancode, char key, uint32_t event) {
    terminal->handler(scancode, key, event);
}

Terminal::Terminal() : width(VGA_WIDTH), height(VGA_HEIGHT), x(0), frame(0), lastUpdateFrame(0), data(),
                       field(reinterpret_cast<VGAEntry *>(0xb8000)) {
    newLine();
}

void Terminal::putChar(VGAEntry c) {
    if (c.ch == '\n') {
        newLine();
    } else {
        currentLine[x] = c;
        x++;
        updateField();
    }
    if (x == width) {
        newLine();
    }
}

void Terminal::shiftUp() {
    frame = min(frame + 1, max(0, data.getSize() - height));
    updateField();
}

void Terminal::shiftDown() {
    frame = max(frame - 1, 0);
    updateField();
}

void Terminal::newLine() {
    currentLine = new VGAEntry[width];
    for (int i = 0; i < width; ++i) {
        currentLine[i] = VGAEntry{' ', VGAColor{BLACK, BLACK}};
    }
    data.push_back(currentLine);
    if (data.getSize() <= frame + height + 1) {
        shiftUp();
    }
    x = 0;
}

void Terminal::updateField() {
    if (frame != lastUpdateFrame) {
        for (int i = frame; i < min(data.getSize(), frame + height); ++i) {
            memcpy(data[i], field + (i - frame) * width, sizeof(VGAEntry) * width);
        }
        lastUpdateFrame = frame;
    } else {
        if (frame + height >= data.getSize())
            memcpy(currentLine, field + width * (height - 1), sizeof(VGAEntry) * width);
    }
}

void Terminal::handler(uint8_t scancode, char key, uint32_t event) {
    (void) key;
    if (event == KEYBOARD_KEY_PRESSED || event == KEYBOARD_KEY_REPEAT) {
        if (scancode == K_CURSOR_UP) {
            shiftDown();
        }
        if (scancode == K_CURSOR_DOWN) {
            shiftUp();
        }
    }
}
