// Nozomi Engine
// game_player.c

#include "game_gfx.h"
#include "game_input.h"
#include "game_object.h"
#include "game_player.h"
#include "game_video.h"

uint8_t num_players;
player_t players[MAX_PLAYERS];

void P_CreatePlayer(uint32_t x, uint32_t y, uint8_t dir)
{
	memset(&players[num_players], 0, sizeof(player_t));
	players[num_players].object = OBJ_CreateObject(x, y, OBJ_NULL);
	players[num_players].object->dir_layer = dir;
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
