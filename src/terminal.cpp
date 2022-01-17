#include <stddef.h>
#include "terminal.h"
#include "string.h"
#include "stdarg.h"

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static inline constexpr uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

static inline constexpr uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t) uc | (uint16_t) color << 8;
}

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t *terminal_buffer;

void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t *) 0xB8000;
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_shift_up() {
    memcpy(terminal_buffer + VGA_WIDTH, terminal_buffer, VGA_WIDTH * (VGA_HEIGHT - 1) * sizeof(uint16_t));
    memset(terminal_buffer + VGA_WIDTH * (VGA_HEIGHT - 1), 0, VGA_WIDTH * sizeof(uint16_t));
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    terminal_buffer[y * VGA_WIDTH + x] = vga_entry(c, color);
}

void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        terminal_row++;
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }
    if (terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        terminal_row++;
    }
    if (terminal_row == VGA_HEIGHT) {
        terminal_shift_up();
        terminal_row--;
    }
}

void terminal_write(const char *data, size_t size) {
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(char *data) {
    terminal_write(data, strlen(data));
}

#define INT32_STRING_MAX_LEN 32

void terminal_print_int32(uint32_t val, int radix, bool full) {
    char str[INT32_STRING_MAX_LEN];
    for (int i = INT32_STRING_MAX_LEN - 1; i >= 0; --i) {
        str[i] = val % radix;
        if (str[i] >= 0xa) {
            str[i] += 'a' - 0xa;
        } else {
            str[i] += '0';
        }
        val /= radix;
        if (!full && val == 0) {
            terminal_write(&str[i], INT32_STRING_MAX_LEN - i);
            return;
        }
    }
    switch (radix) {
        case 8:
            terminal_write(&str[INT32_STRING_MAX_LEN - 11], 11);
            break;
        case 10:
            terminal_write(&str[INT32_STRING_MAX_LEN - 10], 10);
            break;
        case 16:
            terminal_write(&str[INT32_STRING_MAX_LEN - 8], 8);
            break;
        default:
            terminal_write(str, INT32_STRING_MAX_LEN);
    }
}

void terminal_printf(const char *format, ...) {
    pr_list list;
    _pr_start_asm(&list, &format);
    int arg = 0;
    int len = 0;
    bool full = false;
    while (*format) {
        if (*format == '%') {
            format++;
            uint32_t pr_arg = _pr_arg_asm(list, arg);
            arg++;
            if (*format == '[') {
                format++;
                while (*format != ']') {
                    len = len * 10 + *format - '0';
                    format++;
                }
                format++;
            }
            if (*format == '!') {
                full = true;
                format++;
            }
            switch (*format) {
                case 'c':
                    terminal_putchar((char) pr_arg);
                    break;
                case 'd':
                case 'i':
                    terminal_print_int32(pr_arg, 10, full);
                    break;
                case 's':
                    if (len == 0) {
                        terminal_writestring((char *) pr_arg);
                    } else {
                        terminal_write((char *) pr_arg, len);
                    }
                    break;
                case 'x':
                    terminal_print_int32(pr_arg, 16, full);
                    break;
                case '%':
                    terminal_putchar('%');
                    arg--;
                    break;
            }
            len = 0;
            full = false;
        } else {
            terminal_putchar(*format);
        }
        format++;
    }
}