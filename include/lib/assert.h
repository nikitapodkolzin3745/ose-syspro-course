#ifndef OSE_ASSERT_H
#define OSE_ASSERT_H

#include "lib/kernel_panic.h"

#define assert(cond) \
    if (!(cond)) { \
        kernel_panic("Assertion failed: %s at (%s, line #%d)\n", #cond, __FILE__, __LINE__); \
    }

#endif