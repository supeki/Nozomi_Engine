// Nozomi Engine
// game_misc.c

// Misc functions, variables, etc.
// Some useful, some maybe not. Nozomi

#include "game_defs.h"

char *va(const char *fmt, ...) {
    static char buf[4][256];
    static int idx = 0;
    va_list ap;

    idx = (idx + 1) & 3; // Rotate through 4 buffers (0 to 3)
    va_start(ap, fmt);
    _vsnprintf(buf[idx], 256, fmt, ap);
    va_end(ap);

    return buf[idx];
}