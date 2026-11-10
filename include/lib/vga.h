#ifndef OSE_VGA_H
#define OSE_VGA_H

#include <stdarg.h>
#include <stddef.h>

void init_printer();
void put_char(char);
void vprintf(char*, va_list);
void printf(char*, ...);
void vga_set_char(char, size_t, size_t);
void vga_set_color(char, size_t, size_t);
void vga_set_default_color(char);
#define COLOR_WHITE 0x0F
#define COLOR_GREEEN 0x02
#define BACK_COLOR_RED 0x40

#endif
