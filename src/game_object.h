// JADEFRACTURE
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"

#define MAX_OBJECTS 512

typedef struct object_s
{
	// base variables
	int type; // Object type
	struct object_s *prev; // Previous object
	struct object_s *next; // Next object
	subpixel_t x; // X position relative to the world
	subpixel_t y; // Y position relative to the world
	int dir; // Facing direction 0 is down, 3 is right
	subpixel_t hit[4]; // Hitbox, index 0 is x offset, 1 is y offset, 2 is width, 3 is height
	
	// momentum variables
	subpixel_t momx;
	subpixel_t momy;
	
	// animation variables
	int anim_state; // animation state, standing, walking, y'want it y'make it a constant
	int anim_timer; // animation timer, value depends on the animation and situation

	// player reference pointer
	struct player_s *player;
} object_t;

extern object_t objects;

typedef enum
{
	OBJ_NULL,
	OBJ_MARIL // ts bitch playable or something
} objecttypes_e;

void OBJ_InitObjects(void);
void OBJ_RunObjects(void);
object_t *OBJ_CreateObject(subpixel_t x, subpixel_t y, int type);
void OBJ_RemoveObject(object_t *obj);

void PlayerLogic(object_t *maril);

#endif
