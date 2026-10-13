#include <stddef.h>
#include "lib/string.h"
#include <stdarg.h>
#include "lib/math.h"

size_t cursor_x;
size_t cursor_y;

void vga_clear_screen() {
    memzero((void*) 0xB8000, (size_t) (0xB8FA0 - 0xB8000));
}

void vga_print_char(char c, size_t x, size_t y) {
    *(char*) (0xB8000 + 2 * (y * 80 + x) + 1) |= 0x0F;
    *(char*) (0xB8000 + 2 * (y * 80 + x)) = c;
}

void vga_scroll_down() {
    memmove((void*) 0xB8000, (void*) 0xB8000 + 160, (size_t) (0xB8FA0 - 0xB8000 - 160));
    memzero((void*) 0xB8F00, 160);
}

void init_printer() {
    vga_clear_screen();
    cursor_x = 0;
    cursor_y = 0;
}

void put_char(char c) {
    vga_print_char(c, cursor_x, cursor_y);
    if (++cursor_x == 80) {
        cursor_x = 0;
        if (++cursor_y == 25) {
            vga_scroll_down();
            cursor_y = 24;
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
            if (++cursor_y == 25) {
                vga_scroll_down();
                cursor_y = 24;
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