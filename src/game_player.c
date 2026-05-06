// JADEFRACTURE
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
	
	// o'tayyy doing this for save
	int8_t xmove = G_ControlDown(CON_RIGHT, false) - G_ControlDown(CON_LEFT, false);
	int8_t ymove = G_ControlDown(CON_DOWN, false) - G_ControlDown(CON_UP, false);
		
	if (G_ControlDown(CON_LEFT, false) && !G_ControlDown(CON_RIGHT, false)) {
		if (!ymove || maril->dir == 3)
		maril->dir = 1;
	}
	
	if (G_ControlDown(CON_RIGHT, false) && !G_ControlDown(CON_LEFT, false)) {
		if (!ymove || maril->dir == 1)
		maril->dir = 3;
	}
	
	maril->momx = (xmove*(2<<SUBPIXEL_SHIFT));
	
	if (G_ControlDown(CON_UP, false) && !G_ControlDown(CON_DOWN, false)) {
		if (!xmove || maril->dir == 0)
		maril->dir = 2;
	}
	
	if (G_ControlDown(CON_DOWN, false) && !G_ControlDown(CON_UP, false)) {
		if (!xmove || maril->dir == 2)
		maril->dir = 0;
	}
	
	maril->momy = (ymove*(2<<SUBPIXEL_SHIFT));
	
	if (abs(xmove) && abs(ymove)) {
		maril->momx = (maril->momx / 10 * 7);
		maril->momy = (maril->momy / 10 * 7);
	}
		
	if (abs(maril->momx) + abs(maril->momy) > 0) {
		maril->x += maril->momx;
		maril->y += maril->momy;
		
		maril->anim_timer++;
		V_DrawCropped(gfx_characters, TO_PIXELS(maril->x), TO_PIXELS(maril->y), walk_table[(maril->anim_timer/6) % 4]*24, maril->dir*32, 24, 32);
	} else {
		maril->anim_timer = 0;
		V_DrawCropped(gfx_characters, TO_PIXELS(maril->x), TO_PIXELS(maril->y), 0, maril->dir*32, 24, 32);
	}
}
