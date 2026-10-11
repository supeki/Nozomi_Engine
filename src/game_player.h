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

void P_CreatePlayer(int32_t x, int32_t y, int32_t z, uint16_t angle);
void P_RemovePlayer(void);
void P_PlayerLogic(player_t player);

#endif
