// JADEFRACTURE
// game_main.h

#include "game_defs.h"

#ifndef GAME_MAIN_H
#define GAME_MAIN_H

extern bool game_quit;

void gameMain(void);
void gameLoop(void);
void gameRunStuff(uint32_t elapsed);

#endif