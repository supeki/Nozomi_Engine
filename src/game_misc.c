// Nozomi Engine
// game_misc.c

// Misc functions, variables, etc.
// Some useful, some maybe not. Nozomi

#include "game_defs.h"
#include "i_system.h"

#define VA_BUF_SIZE 256

char *va(const char *fmt, ...) {
    static char buf[4][VA_BUF_SIZE];
    static int idx = 0;
    va_list ap;

    idx = (idx + 1) & 3;
    va_start(ap, fmt);
    vsnprintf(buf[idx], 256, fmt, ap);
    va_end(ap);

    return buf[idx];
}

void DF_Free(dirfiles_t *dirfiles)
{
    if (dirfiles != NULL) {
        free(dirfiles->directories);
        free(dirfiles->filenames);
        dirfiles->directories = NULL;
        dirfiles->filenames = NULL;
        dirfiles->num_files = 0;
        dirfiles->num_dirs = 0;
    }
}
