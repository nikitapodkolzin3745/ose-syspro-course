#include <stddef.h>
#include "lib/vga.h"
#include "lib/kernel_panic.h"
#include "lib/assert.h"
#include "lib/allocator.h"
#include "lib/types.h"
#include "system/interrupts.h"

void kernel() {
    init_printer();
    vga_set_default_color(COLOR_GREEEN);

    init_interrupts();

    // Exeption
    // __asm__ volatile (
    //     "xorl %%ecx, %%ecx\n"
    //     "divl %%ecx"
    //     :
    //     :
    //     : "eax", "ecx", "edx", "cc"
    // );

    // System call
    // __asm__ volatile (
    //     "int $42"
    //     :
    //     :
    //     : "eax", "cc"
    // );

    // I/O Interrupt
    __asm__ volatile (
        "sti\n"
        ".extern infinite_loop\n"
        "jmp infloop\n"
        :
        :
        : "cc"
    );

    kernel_panic("Nothing to do...");
}