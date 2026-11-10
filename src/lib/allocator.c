#include <stddef.h>
#include "lib/kernel_panic.h"

#define arena ((void*) 0x100000)
#define arena_size 0x100000
#define arena_end ((void*) ((size_t) arena + arena_size))

#define alignment(x, a) ((void*) (((size_t) x + a - 1) & ~(a - 1)))

static void* cursor = arena;

void* malloc_immortal(size_t size, size_t alignment) {
    if (alignment(cursor, alignment) + size > arena_end)
        kernel_panic("Out of memory in malloc_immortal");
    void* ptr = alignment(cursor, alignment);
    cursor = ptr + size;
    return ptr;
}