#include <stddef.h>
#include "lib/string.h"
#include <stdarg.h>

#define VGA_BUFFER 0xB8000
#define VGA_SIZE 0xFA0
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define COLOR_WHITE 0x0F

static size_t cursor_x;
static size_t cursor_y;

void vga_clear_screen() {
    memzero((void*) VGA_BUFFER, VGA_SIZE);
}

void vga_print_char(char c, size_t x, size_t y) {
    *(char*) (VGA_BUFFER + 2 * (y * VGA_WIDTH + x) + 1) |= COLOR_WHITE;
    *(char*) (VGA_BUFFER + 2 * (y * VGA_WIDTH + x)) = c;
}

void vga_scroll_down() {
    memmove((void*) VGA_BUFFER, (void*) VGA_BUFFER + 2 * VGA_WIDTH, (size_t) (VGA_BUFFER + VGA_SIZE - VGA_BUFFER - 2 * VGA_WIDTH));
    memzero((void*) (VGA_BUFFER + VGA_SIZE - 2 * VGA_WIDTH), 2 * VGA_WIDTH);
}

void init_printer() {
    vga_clear_screen();
    cursor_x = 0;
    cursor_y = 0;
}

void put_char(char c) {
    vga_print_char(c, cursor_x, cursor_y);
    if (++cursor_x == VGA_WIDTH) {
        cursor_x = 0;
        if (++cursor_y == VGA_HEIGHT) {
            vga_scroll_down();
            cursor_y = VGA_HEIGHT - 1;
        }
    }
}

static void put_unsigned_decimal(unsigned int value) {
    unsigned int divisor = 1;
    while (value / divisor >= 10)
        divisor *= 10;

    do {
        put_char('0' + value / divisor);
        value %= divisor;
        divisor /= 10;
    } while (divisor);
}

void vprintf(const char* fmt, va_list args) {
    for (size_t i = 0; fmt[i] != '\0'; i++) {
        if (fmt[i] == '%') {
            switch (fmt[++i]) {
                case 'c': {
                    put_char((char) va_arg(args, int));
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    for (size_t j = 0; str[j] != '\0'; j++)
                        put_char(str[j]);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    unsigned int value;
                    if (num < 0) {
                        put_char('-');
                        value = 0u - (unsigned int) num;
                    } else {
                        value = (unsigned int) num;
                    }
                    put_unsigned_decimal(value);
                    break;
                }
                case 'x': {
                    unsigned int num = va_arg(args, unsigned int);
                    put_char('0');
                    put_char('x');
                    for (int j = 7; j >= 0; j--) {
                        unsigned int nibble = (num >> (j * 4)) & 0xF;
                        if (nibble < 10)
                            put_char('0' + nibble);
                        else
                            put_char('A' + (nibble - 10));
                    }
                    break;
                }
            }
        } else
        if (fmt[i] == '\n') {
            cursor_x = 0;
            if (++cursor_y == VGA_HEIGHT) {
                vga_scroll_down();
                cursor_y = VGA_HEIGHT - 1;
            }
        } else
        if (fmt[i] == '\r')
            cursor_x = 0;
        else
            put_char(fmt[i]);
    }
}

void printf(char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
}