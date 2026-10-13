#include <stdarg.h>
#include "lib/vga.h"

void infloop();
void do_cli();

void vkernel_panic(char* fmt, va_list args) {
    do_cli();
    vprintf(fmt, args);
    infloop();
}

void kernel_panic(char* fmt, ...) {;
    va_list args;
    va_start(args, fmt);
    vkernel_panic(fmt, args);
    va_end(args);
}