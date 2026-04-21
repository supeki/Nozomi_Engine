// JADEFRACTURE
// i_system.h

#include "game_defs.h"

#ifndef I_SYSTEM_H
#define I_SYSTEM_H

uint32_t I_GetTicks(void);
uint32_t I_GetTime(void);
void I_Sleep(uint32_t ms);
void I_Error (char *error, ...);

#endif