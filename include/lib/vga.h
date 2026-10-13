#ifndef OSE_VGA_H
#define OSE_VGA_H

#include <stdarg.h>

void init_printer();
void put_char(char);
void vprintf(char*, va_list);
void printf(char*, ...);

#endif
