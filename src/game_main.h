// Nozomi Engine
// game_main.h

#ifndef GAME_MAIN_H
#define GAME_MAIN_H

#include "game_defs.h"

extern bool game_quit;
extern uint32_t game_tick;

void gameMain(void);
void gameLoop(void);
void gameRunStuff(uint32_t elapsed);
void gameDisplay(void);

#endif
