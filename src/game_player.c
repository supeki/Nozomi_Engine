// Nozomi Engine
// game_player.c

#include "game_gfx.h"
#include "game_input.h"
#include "game_object.h"
#include "game_player.h"
#include "game_video.h"

uint8_t num_players;
player_t players[MAX_PLAYERS];

void P_CreatePlayer(int32_t x, int32_t y, int32_t z, uint16_t angle)
{
	memset(&players[num_players], 0, sizeof(player_t));
	players[num_players].object = OBJ_CreateObject(x, y, z, OBJ_PLAYER);
	players[num_players].object->angle = angle;
	players[num_players].object->player = &players[num_players];
	
	num_players++;
}

void P_RemovePlayer(void)
{
	num_players--;
	memset(&players[num_players], 0, sizeof(player_t));
	OBJ_RemoveObject(players[num_players].object);
}

void P_PlayerLogic(player_t player)
{
}
