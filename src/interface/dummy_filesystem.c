// Nozomi Engine
// dummy filesystem backend
// dummy_filesystem.c

#include "../game_defs.h"
#include "../i_system.h"

const char *I_GetHomeDir(void)
{
    return ".";
}

dirfiles_t I_GetDir(const char *path)
{
    dirfiles_t dirfiles;

    // init dirfiles to prevent issues
    memset(&dirfiles, 0, sizeof(dirfiles_t));

    // go ahead and return cuz dummy
    return dirfiles;
}