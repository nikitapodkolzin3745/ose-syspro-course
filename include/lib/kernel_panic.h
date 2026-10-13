#ifndef OSE_KERNEL_PANIC_H
#define OSE_KERNEL_PANIC_H

#include <stdarg.h>

void kernel_panic(char*, ...);
void vkernel_panic(char*, va_list);

#endif
