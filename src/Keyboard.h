#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_KEY_RELEASED 1
#define KEYBOARD_KEY_PRESSED 2
#define KEYBOARD_KEY_REPEAT 3

typedef void(*KeyboardEventHandler)(uint8_t scancode, char key, uint32_t event);

void KeyboardGlobalEventHandler();

void KeyboardSetLocalEventHandler(KeyboardEventHandler handler);

#endif //KEYBOARD_H
