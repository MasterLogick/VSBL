#ifndef BASIC_STREAM_C
#define BASIC_STREAM_C

#include <stdint.h>
#include "VGAEntry.h"
#include <traits/NumberTraits.h>
#include <string.h>
#include <traits/TemplateTraits.h>
#include "Terminal.h"

class basic_stream {
public:
    enum Mode {
        BINARY = 2, OCTAL = 8, DECIMAL = 10, HEX = 16
    };

    static struct Small {
    } small;

    static struct Big {
    } big;

    static struct Full {
    } full;

    static struct Trimmed {
    } trimmed;
    static struct Defaults {
    } defaults;

private:

    VGAColor terminalColor;
    Mode mode;
    bool modSmall;
    bool modFull;

    template<typename T>
    void printNumber(T num) {
        int size = 0;
        switch (mode) {
            case BINARY:
                size = NumberTraits<T>::width;
                break;
            case OCTAL:
                size = (NumberTraits<T>::width + 2) / 3;
                break;
            case DECIMAL:
                size = STRLEN(NumberTraits<T>::max);
                break;
            case HEX:
                size = NumberTraits<T>::width / 4;
                break;
            default:
                size = 1;
                break;
        }
        if (NumberTraits<T>::sign) {
            size--;
            if (num < 0) {
                terminal->putChar(VGAEntry{'-', terminalColor});
            }
        }
        char str[size];
        int i;
        for (i = size - 1; i >= 0; --i) {
            str[i] = num % mode + '0';
            if (str[i] > '9') {
                str[i] += (modSmall ? 'a' : 'A') - '9' - 1;
            }
            num /= mode;
            if (!num && !modFull) {
                i--;
                break;
            }
        }
        for (i += 1; i < size; ++i) {
            terminal->putChar(VGAEntry{static_cast<uint8_t>(str[i]), terminalColor});
        }
    }

public:
    constexpr basic_stream() : basic_stream(BLACK, LIGHT_GREY) {}

    constexpr basic_stream(Color background, Color foreground) : terminalColor{foreground, background},
                                                                 mode(DECIMAL), modSmall(true), modFull(false) {}

    basic_stream &operator<<(bool c);

    basic_stream &operator<<(char c);

    basic_stream &operator<<(short c);

    basic_stream &operator<<(int c);

    basic_stream &operator<<(long c);

    basic_stream &operator<<(unsigned char c);

    basic_stream &operator<<(unsigned short c);

    basic_stream &operator<<(unsigned int c);

    basic_stream &operator<<(unsigned long c);

    basic_stream &operator<<(char *c);

    basic_stream &operator<<(const char *c);

    template<typename T>
    basic_stream &operator<<(T *c) {
        operator<<(HEX);
        operator<<(full);
        operator<<((uint64_t) c);
        operator<<(defaults);
        return *this;
    }

    basic_stream &operator<<(Big);

    basic_stream &operator<<(Small);

    basic_stream &operator<<(Full);

    basic_stream &operator<<(Trimmed);

    basic_stream &operator<<(Mode m);

    basic_stream &operator<<(Defaults);
};

extern basic_stream cout;

const char endl = '\n';

#endif //BASIC_STREAM_C