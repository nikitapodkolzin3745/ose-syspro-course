#include <stddef.h>
#include "lib/vga.h"
#include "lib/kernel_panic.h"
#include "lib/assert.h"
#include "lib/allocator.h"
#include "lib/types.h"
#include "system/interrupts.h"

static volatile u32 zero_divisor;

void kernel() {
    init_printer();
    vga_set_default_color(COLOR_GREEEN);

    init_interrupts();

    // Exeption
    __asm__ volatile (
        "movl $0x11111111, %%eax\n"
        "movl $0x22222222, %%ecx\n"
        "movl $0x33333333, %%edx\n"
        "movl $0x44444444, %%ebx\n"
        "movl $0x55555555, %%esi\n"
        "movl $0x66666666, %%edi\n"
        "divl %0"
        :
        : "m" (zero_divisor)
        : "eax", "ecx", "edx", "ebx", "esi", "edi", "cc"
    );

    // System call
    __asm__ volatile (
        "movl $0x11111111, %%eax\n"
        "movl $0x22222222, %%ecx\n"
        "movl $0x33333333, %%edx\n"
        "movl $0x44444444, %%ebx\n"
        "movl $0x55555555, %%esi\n"
        "movl $0x66666666, %%edi\n"
        "int $42"
        :
        :
        : "eax", "ecx", "edx", "ebx", "esi", "edi", "cc"
    );

    // I/O Interrupt
    __asm__ volatile (
        "movl $0x11111111, %%eax\n"
        "movl $0x22222222, %%ecx\n"
        "movl $0x33333333, %%edx\n"
        "movl $0x44444444, %%ebx\n"
        "movl $0x55555555, %%esi\n"
        "movl $0x66666666, %%edi\n"
        "sti\n"
        ".extern infinite_loop\n"
        "jmp infloop\n"
        :
        :
        : "eax", "ecx", "edx", "ebx", "esi", "edi", "cc"
    );

    kernel_panic("Nothing to do...");
}