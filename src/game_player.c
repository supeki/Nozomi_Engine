// Nozomi Engine
// game_player.c

#include "game_gfx.h"
#include "game_input.h"
#include "game_object.h"
#include "game_player.h"
#include "game_video.h"

static int walk_table[4] = {1, 0, 2, 0};

uint8_t num_players;
player_t players[4];

void P_CreatePlayer(subpixel_t x, subpixel_t y, uint8_t dir)
{
	memset(&players[num_players], 0, sizeof(player_t));
	players[num_players].object = OBJ_CreateObject(x, y, OBJ_MARIL);
	players[num_players].object->dir = dir;
	
	num_players++;
}

void P_RemovePlayer(void)
{
	num_players--;
	memset(&players[num_players], 0, sizeof(player_t));
	players[num_players].object = (void *)NULL;
}

void P_PlayerLogic(player_t player)
{
	object_t *maril = player.object;
}
