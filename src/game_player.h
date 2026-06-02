// Nozomi Engine
// game_player.h

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "game_defs.h"
#include "game_object.h"

typedef struct player_s
{
	uint8_t hp; // total max is 99 soooooo
	
	object_t *object;
} player_t;

extern uint8_t num_players;
extern player_t players[4];

// P for Player!

void P_CreatePlayer(subpixel_t x, subpixel_t y, uint8_t dir);
void P_RemovePlayer(void);
void P_PlayerLogic(player_t player);

#endif
