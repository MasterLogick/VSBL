#include "basic_stream.h"

basic_stream cout;

basic_stream &basic_stream::operator<<(bool c) {
    if (c) {
        operator<<("true");
    } else {
        operator<<("false");
    }
    return *this;
}

basic_stream &basic_stream::operator<<(char c) {
    terminal->putChar(VGAEntry{static_cast<uint8_t>(c), terminalColor});
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
        terminal->putChar(VGAEntry{static_cast<uint8_t>(*c), terminalColor});
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
