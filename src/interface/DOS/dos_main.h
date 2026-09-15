// Nozomi Engine
// DOS/DJGPP backend
// dos_main.h

#ifndef DOS_MAIN_H
#define DOS_MAIN_H

#include "../../game_defs.h"

extern uint32_t timer_ticks;
extern int mouse_offx, mouse_offy;

void I_Ticker(void);

#endif