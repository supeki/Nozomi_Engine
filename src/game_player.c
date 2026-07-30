// Nozomi Engine
// game_player.c

#include "game_gfx.h"
#include "game_input.h"
#include "game_object.h"
#include "game_player.h"
#include "game_video.h"

uint8_t num_players;
player_t players[MAX_PLAYERS];

void P_CreatePlayer(subpixel_t x, subpixel_t y, uint8_t dir)
{
	memset(&players[num_players], 0, sizeof(player_t));
	players[num_players].object = OBJ_CreateObject(x, y, OBJ_MARIL);
	players[num_players].object->dir = dir;
	players[num_players].object->player = &players[num_players];
	
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
	
	int move_speed = PU/2*3;
	
	if (G_ControlDown(PLAYER_ONE, CON_B, false))
		move_speed = PU/2*5;

	bool moving = (
		G_ControlDown(PLAYER_ONE, CON_LEFT, false) 
		|| G_ControlDown(PLAYER_ONE, CON_RIGHT, false) 
		|| G_ControlDown(PLAYER_ONE, CON_UP, false) 
		|| G_ControlDown(PLAYER_ONE, CON_DOWN, false)
	);
	
	subpixel_t move_x = 0, move_y = 0;

	if (G_ControlDown(PLAYER_ONE, CON_LEFT, false)) {
		move_x-=move_speed;
		
		if (!(G_ControlDown(PLAYER_ONE, CON_UP, false) || G_ControlDown(PLAYER_ONE, CON_DOWN, false)) || maril->dir == 3)
			maril->dir = 1;
	}

	if (G_ControlDown(PLAYER_ONE, CON_RIGHT, false)) {
		move_x+=move_speed;

		if (!(G_ControlDown(PLAYER_ONE, CON_UP, false) || G_ControlDown(PLAYER_ONE, CON_DOWN, false)) || maril->dir == 1)
			maril->dir = 3;
	}
	
	if (G_ControlDown(PLAYER_ONE, CON_UP, false)) {
		move_y-=move_speed;
		
		if (!(G_ControlDown(PLAYER_ONE, CON_LEFT, false) || G_ControlDown(PLAYER_ONE, CON_RIGHT, false)) || maril->dir == 0)
			maril->dir = 2;
	}

	if (G_ControlDown(PLAYER_ONE, CON_DOWN, false)) {
		move_y+=move_speed;
	
		if (!(G_ControlDown(PLAYER_ONE, CON_LEFT, false) || G_ControlDown(PLAYER_ONE, CON_RIGHT, false)) || maril->dir == 2)
			maril->dir = 0;
	}
	
	if (!OBJ_TryMovement(maril, move_x, move_y))
		moving = false;
	
	if (G_ControlDown(PLAYER_ONE, CON_A, true)) //only on press
	{
		subpixel_t check_x, check_y, move_x, move_y;
		
		switch (maril->dir) {
			case 0: // down
				check_x = maril->x + maril->hit[0]/2;
				check_y = maril->y + maril->hit[1]/2 + maril->hit[3];
				move_x = 0;
				move_y = PU;
				break;
			case 1: // left
				check_x = maril->x - maril->hit[0]/2;
				check_y = maril->y + maril->hit[1];
				move_x = -PU;
				move_y = 0;
				break;
			case 2: // up
				check_x = maril->x + maril->hit[0]/2;
				check_y = maril->y + maril->hit[1]/2;
				move_x = 0;
				move_y = -PU;
				break;
			default: // right
				check_x = maril->x + maril->hit[0]/2 + maril->hit[2];
				check_y = maril->y + maril->hit[1];
				move_x = PU;
				move_y = 0;
				break;
		}
	
		object_t *check_obj = OBJ_CreateObject(check_x, check_y, OBJ_CHECK);
		OBJ_TryMovement(check_obj, move_x, move_y);
	}
	
	camera.y = maril->y/PU - VID_HEIGHT/2 + maril->hit[3]/(2*PU) + maril->hit[1]/(2*PU);
	
	if (camera.y < 0)
		camera.y = 0;
	if (camera.y > 576 - VID_HEIGHT)
		camera.y = 576 - VID_HEIGHT;

	if (moving) {
		maril->anim_timer++;
		if (G_ControlDown(PLAYER_ONE, CON_B, false))
			maril->anim_timer++;
	} else {
		maril->anim_timer = 0;
	}
}
