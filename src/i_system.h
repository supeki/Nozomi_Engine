// Nozomi Engine
// i_system.h

#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

#include "game_defs.h"

EXTERNC uint32_t I_GetTicks(void);
EXTERNC uint32_t I_GetTime(void);
EXTERNC void I_Sleep(uint32_t ms);
EXTERNC void I_printf (const char *text, ...);
EXTERNC void I_Error (const char *error, ...);

#ifdef SATURN
EXTERNC void* nMalloc(size_t size);
#endif

#endif
