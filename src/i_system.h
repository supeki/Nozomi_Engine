// Nozomi Engine
// i_system.h

#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include "game_defs.h"

uint32_t I_GetTicks(void);
uint32_t I_GetTime(void);
void I_Sleep(uint32_t ms);
void I_printf (const char *text, ...);
void I_Error (const char *error, ...);

// pretend this is some sort of i_filesystem.h

typedef struct
{
    char **filenames;
    uint32_t num_files;

    char **directories;
    uint32_t num_dirs;
} dirfiles_t;

const char *I_GetHomeDir(void);
dirfiles_t I_GetDir(const char *path);

#endif
