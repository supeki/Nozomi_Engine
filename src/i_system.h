// JADEFRACTURE
// i_system.h

#ifndef I_SYSTEM_H
#define I_SYSTEM_H

#include "game_defs.h"

uint32_t I_GetTicks(void);
uint32_t I_GetTime(void);
void I_Sleep(uint32_t ms);
void I_Error (char *error, ...);

#endif