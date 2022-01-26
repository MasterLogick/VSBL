#include "basic_stream.h"

basic_stream cout;

VGAEntry *const terminal = reinterpret_cast<VGAEntry *>(0xB8000);

void basic_stream::putChar(uint8_t c) {
    if (c == '\n') {
        x = 0;
        y++;
    } else {
        insertEntry(VGAEntry{c, terminalColor}, x, y);
        x++;
    }
    if (x == width) {
        x = 0;
        y++;
    }
    if (y == height) {
        shiftUp();
        y--;
    }
}

void basic_stream::insertEntry(VGAEntry entry, int x, int y) {
    terminal[x + y * width] = entry;
}

void basic_stream::shiftUp() {
    for (int i = 0; i < width * (height - 1); ++i) {
        terminal[i] = terminal[i + width];
    }
    memset(terminal + (width) * (height - 1), 0, sizeof(VGAEntry) * width);
}

basic_stream &basic_stream::operator<<(bool c) {
    if (c) {
        operator<<("true");
    } else {
        operator<<("false");
    }
    return *this;
}

basic_stream &basic_stream::operator<<(char c) {
    putChar(c);
    return *this;
}

basic_stream &basic_stream::operator<<(short c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(int c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(long c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(unsigned char c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(unsigned short c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(unsigned int c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(unsigned long c) {
    printNumber(c);
    return *this;
}

basic_stream &basic_stream::operator<<(char *c) {
    operator<<(static_cast<const char *>(c));
    return *this;
}

basic_stream &basic_stream::operator<<(const char *c) {
    while (*c) {
        putChar(*c);
        c++;
    }
    return *this;
}

basic_stream &basic_stream::operator<<(basic_stream::Big) {
    modSmall = false;
    return *this;
}

basic_stream &basic_stream::operator<<(basic_stream::Small) {
    modSmall = true;
    return *this;
}

basic_stream &basic_stream::operator<<(basic_stream::Full) {
    modFull = true;
    return *this;
}

basic_stream &basic_stream::operator<<(basic_stream::Trimmed) {
    modFull = false;
    return *this;
}

basic_stream &basic_stream::operator<<(Mode m) {
    mode = m;
    return *this;
}

basic_stream &basic_stream::operator<<(basic_stream::Defaults) {
    mode = DECIMAL;
    modFull = false;
    modSmall = true;
    return *this;
}
