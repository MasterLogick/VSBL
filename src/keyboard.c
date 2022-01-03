#include "keyboard.h"
#include <stdbool.h>
#include "ps2.h"

#define KEY_COUNT 0x100

bool keyboard[KEY_COUNT];
char lookup_table[KEY_COUNT] = {
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

//todo use list for multiple handlers
keyboard_event_handler local_handler;

#define KEYBOARD_FIRST_BLOCK_END 0x79
#define KEYBOARD_SECOND_BLOCK_START 0x80
#define KEYBOARD_SECOND_BLOCK_END 0xff

#define K_SECOND_PART_SCANCODE 0xE0
#define K_SECOND_PART_PRESSED K_SECOND_PART_SCANCODE

#define K_ESCAPE 0x01
#define K_ONE 0x02
#define K_TWO 0x03
#define K_THREE 0x04
#define K_FOUR 0x05
#define K_FIVE 0x06
#define K_SIX 0x07
#define K_SEVEN 0x08
#define K_EIGHT 0x09
#define K_NINE 0x0A
#define K_ZERO 0x0B
#define K_MINUS 0x0C
#define K_EQUAL_SIGN 0x0D
#define K_BACKSPACE 0x0E
#define K_TAB 0x0F
#define K_Q 0x10
#define K_W 0x11
#define K_E 0x12
#define K_R 0x13
#define K_T 0x14
#define K_Y 0x15
#define K_U 0x16
#define K_I 0x17
#define K_O 0x18
#define K_P 0x19
#define K_LEFT_SQUARE_BRACKET 0x1A
#define K_RIGHT_SQUARE_BRACKET 0x1B
#define K_ENTER 0x1C
#define K_LEFT_CONTROL 0x1D
#define K_A 0x1E
#define K_S 0x1F
#define K_D 0x20
#define K_F 0x21
#define K_G 0x22
#define K_H 0x23
#define K_J 0x24
#define K_K 0x25
#define K_L 0x26
#define K_SEMICOLON 0x27
#define K_SINGLE_QUOTE 0x28
#define K_BACK_TICK 0x29
#define K_LEFT_SHIFT 0x2A
#define K_BACKSLASH 0x2B
#define K_Z 0x2C
#define K_X 0x2D
#define K_C 0x2E
#define K_V 0x2F
#define K_B 0x30
#define K_N 0x31
#define K_M 0x32
#define K_COLON 0x33
#define K_FULL_STOP 0x34
#define K_SLASH 0x35
#define K_RIGHT_SHIFT 0x36
#define K_KEYPAD_STAR 0x37
#define K_LEFT_ALT 0x38
#define K_SPACE 0x39
#define K_CAPS_LOCK 0x3A
#define K_F_ONE 0x3B
#define K_F_TWO 0x3C
#define K_F_THREE 0x3D
#define K_F_FOUR 0x3E
#define K_F_FIVE 0x3F
#define K_F_SIX 0x40
#define K_F_SEVEN 0x41
#define K_F_EIGHT 0x42
#define K_F_NINE 0x43
#define K_F_TEN 0x44
#define K_NUMBER_LOCK 0x45
#define K_SCROLL_LOCK 0x46
#define K_KEYPAD_SEVEN 0x47
#define K_KEYPAD_EIGHT 0x48
#define K_KEYPAD_NINE 0x49
#define K_KEYPAD_MINUS 0x4A
#define K_KEYPAD_FOUR 0x4B
#define K_KEYPAD_FIVE 0x4C
#define K_KEYPAD_SIX 0x4D
#define K_KEYPAD_PLUS 0x4E
#define K_KEYPAD_ONE 0x4F
#define K_KEYPAD_TWO 0x50
#define K_KEYPAD_THREE 0x51
#define K_KEYPAD_ZERO 0x52
#define K_KEYPAD_FULL_STOP 0x53
#define K_KEYPAD_F_ELEVEN 0x57
#define K_KEYPAD_F_TWELVE 0x58

bool global_keyboard_second_block_event = false;

void keyboard_global_handle_event() {
    uint8_t scancode = ps2_read_response();
    if (scancode == K_SECOND_PART_SCANCODE) {
        global_keyboard_second_block_event = true;
        if (local_handler)local_handler(scancode, '\0', K_SECOND_PART_SCANCODE);
        return;
    }
    if (!global_keyboard_second_block_event) {
        if (scancode <= KEYBOARD_FIRST_BLOCK_END) {
            bool pressed = keyboard[scancode];
            keyboard[scancode] = true;
            if (local_handler)
                local_handler(scancode, lookup_table[scancode], pressed ? KEYBOARD_KEY_REPEAT : KEYBOARD_KEY_PRESSED);
        } else {
            keyboard[scancode - KEYBOARD_SECOND_BLOCK_START] = false;
            if (local_handler)
                local_handler(scancode, lookup_table[scancode - KEYBOARD_SECOND_BLOCK_START], KEYBOARD_KEY_RELEASED);
        }
    } else {
        global_keyboard_second_block_event = false;
    }
}

void keyboard_set_local_event_handler(keyboard_event_handler handler) {
    local_handler = handler;
}