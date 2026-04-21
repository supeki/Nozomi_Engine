// JADEFRACTURE
// game_object.h

#include "game_defs.h"

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

typedef struct
{
	// base variables
	int x; // X position relative to the world
	int y; // Y position relative to the world
	int dir; // Facing direction 0 is down, 3 is right
	int hit[4]; // Hitbox, index 0 is x offset, 1 is y offset, 2 is width, 3 is height
	
	// animation variables
	int anim_state; // animation state, standing, walking, y'want it y'make it a constant
	int anim_timer; // animation timer, value depends on the animation and situation
} object_t;

#endif
