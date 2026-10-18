#pragma once

#ifdef DEBAG
#include "lib/kernel_panic.h"

#define assert(cond) \
    if (!(cond)) { \
        kernel_panic("Assertion failed: %s at (%s, line #%d)\n", #cond, __FILE__, __LINE__); \
    }
#else
#define assert(cond) ()
#endif