// JADEFRACTURE
// game_player.h

#ifndef GAME_PLAYER_H
#define GAME_PLAYER_H

#include "game_defs.h"
#include "game_object.h"

typedef struct player_s
{
	uint8_t hp;
	
	object_t *object;
} player_t;

#endif
