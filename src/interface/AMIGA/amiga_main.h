// Nozomi Engine
// Commodore Amiga backend
// amiga_main.h

#ifndef AMIGA_MAIN_H
#define AMIGA_MAIN_H

#include "../../game_defs.h"

extern uint32_t timer_ticks;
extern int mouse_offx, mouse_offy;

void I_StartupTimer(void);
void I_ShutdownTimer(void);

extern struct Window *amiga_window;

#endif