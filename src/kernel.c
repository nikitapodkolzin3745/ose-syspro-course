#include <stddef.h>
#include "lib/vga.h"
#include "lib/kernel_panic.h"
#include "lib/assert.h"
#include "lib/allocator.h"

void infloop();

void kernel() {
    init_printer();

    const size_t sizes[] = {1, 7, 19, 64, 255, 1024, 4097};
    const size_t alignments[] = {1, 4, 8, 16, 64};
    const size_t size_count = sizeof(sizes) / sizeof(sizes[0]);
    const size_t alignment_count = sizeof(alignments) / sizeof(alignments[0]);
    size_t allocations = 0;
    size_t previous_end = 0;

    printf("Immortal allocator test: 7 sizes x 5 alignments\n");
    printf("Allocations are never freed; output should scroll.\n\n");

    while (1) {
        size_t size_index = allocations % size_count;
        size_t alignment_index = allocations % alignment_count;
        size_t size = sizes[size_index];
        size_t alignment = alignments[alignment_index];
        assert(alignment != 0 && (alignment & (alignment - 1)) == 0);

        void* pointer = malloc_immortal(size, alignment);
        if (pointer == NULL)
            kernel_panic("Immortal object arena exhausted after %d allocations (next size=%d, alignment=%d)",
                         (int) allocations, (int) size, (int) alignment);

        assert(((size_t) pointer & (alignment - 1)) == 0);
        if (allocations != 0)
            assert((size_t) pointer >= previous_end);
        printf("#%d size=%d align=%d addr=%x\n",
               (int) allocations, (int) size, (int) alignment,
               (unsigned int) (size_t) pointer);
        previous_end = (size_t) pointer + size;
        allocations++;
    }
}