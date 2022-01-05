#ifndef KEYBOARD_H
#define KEYBOARD_H

#ifdef __cplusplus
extern "C"{
#endif

#include <stdint.h>

#define KEYBOARD_KEY_RELEASED 1
#define KEYBOARD_KEY_PRESSED 2
#define KEYBOARD_KEY_REPEAT 3

typedef void(*keyboard_event_handler)(uint8_t scancode, char key, uint32_t event);

void keyboard_global_handle_event(void);

void keyboard_set_local_event_handler(keyboard_event_handler handler);

#ifdef __cplusplus
}
#endif

#endif //KEYBOARD_H
