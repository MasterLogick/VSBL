#include "Keyboard.h"
#include <ps2.h>
#include <Vector.h>

#define KEY_COUNT 0x100

bool keyboard[KEY_COUNT];
char lookupTable[KEY_COUNT] = {
        0,
        0,
        '1',
        '2',
        '3',
        '4',
        '5',
        '6',
        '7',
        '8',
        '9',
        '0',
        '-',
        '=',
        '\0',
        '\t',
        'Q',
        'W',
        'E',
        'R',
        'T',
        'Y',
        'U',
        'I',
        'O',
        'P',
        '[',
        ']',
        '\n',
        '\0',
        'A',
        'S',
        'D',
        'F',
        'G',
        'H',
        'J',
        'K',
        'L',
        ';',
        '\'',
        '`',
        '\0',
        '\\',
        'Z',
        'X',
        'C',
        'V',
        'B',
        'N',
        'M',
        ',',
        '.',
        '/',
        '\0',
        '*',
        '\0',
        ' ',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '7',
        '8',
        '9',
        '-',
        '4',
        '5',
        '6',
        '+',
        '1',
        '2',
        '3',
        '0',
        '.',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0',
        '\0'
};

Vector<KeyboardEventHandler> keyboardHandlers;

#define KEYBOARD_FIRST_BLOCK_END 0x79
#define KEYBOARD_SECOND_BLOCK_START 0x80
#define KEYBOARD_SECOND_BLOCK_END 0xff

#define K_SECOND_PART_SCANCODE 0xE0
#define K_SECOND_PART_PRESSED K_SECOND_PART_SCANCODE

bool globalKeyboardSecondBlockEvent = false;

void KeyboardGlobalEventHandler() {
    uint8_t scancode = ps2_read_response();
    if (scancode == K_SECOND_PART_SCANCODE) {
        globalKeyboardSecondBlockEvent = true;
        for (int i = 0; i < keyboardHandlers.getSize(); ++i) {
            keyboardHandlers[i](scancode, '\0', K_SECOND_PART_SCANCODE);
        }
        return;
    }
    if (!globalKeyboardSecondBlockEvent) {
        if (scancode <= KEYBOARD_FIRST_BLOCK_END) {
            bool pressed = keyboard[scancode];
            keyboard[scancode] = true;
            for (int i = 0; i < keyboardHandlers.getSize(); ++i) {
                keyboardHandlers[i](scancode, lookupTable[scancode],
                                    pressed ? KEYBOARD_KEY_REPEAT : KEYBOARD_KEY_PRESSED);
            }
        } else {
            keyboard[scancode - KEYBOARD_SECOND_BLOCK_START] = false;
            for (int i = 0; i < keyboardHandlers.getSize(); ++i) {
                keyboardHandlers[i](scancode, lookupTable[scancode - KEYBOARD_SECOND_BLOCK_START],
                                    KEYBOARD_KEY_RELEASED);
            }
        }
    } else {
        if (scancode <= KEYBOARD_FIRST_BLOCK_END) {
            bool pressed = keyboard[scancode + KEYBOARD_SECOND_BLOCK_START];
            keyboard[scancode] = true;
            for (int i = 0; i < keyboardHandlers.getSize(); ++i) {
                keyboardHandlers[i](scancode + KEYBOARD_SECOND_BLOCK_START, '\0',
                                    pressed ? KEYBOARD_KEY_REPEAT : KEYBOARD_KEY_PRESSED);
            }
        } else {
            keyboard[scancode] = false;
            for (int i = 0; i < keyboardHandlers.getSize(); ++i) {
                keyboardHandlers[i](scancode + KEYBOARD_SECOND_BLOCK_START, '\0', KEYBOARD_KEY_RELEASED);
            }
        }
        globalKeyboardSecondBlockEvent = false;
    }
}

void KeyboardAddLocalEventHandler(KeyboardEventHandler handler) {
    if (handler)
        keyboardHandlers.push_back(handler);
}