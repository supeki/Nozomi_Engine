// Nozomi Engine
// game_player.h

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "game_defs.h"
#include "game_object.h"

typedef struct player_s
{
	object_t *object;
} player_t;

extern uint8_t num_players;
extern player_t players[MAX_PLAYERS];

// P for Player!

void P_CreatePlayer(uint32_t x, uint32_t y, uint8_t dir);
void P_RemovePlayer(void);
void P_PlayerLogic(player_t player);

#endif
