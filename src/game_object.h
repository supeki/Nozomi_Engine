// JADEFRACTURE
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"
#include "game_video.h"

#define MAX_OBJECTS 512

typedef struct object_s
{
	// base variables
	int type; // Object type
	struct object_s *prev; // Previous object
	struct object_s *next; // Next object
	subpixel_t x; // X position relative to the world
	subpixel_t y; // Y position relative to the world
	uint8_t dir; // Facing direction 0 is down, 3 is right
	subpixel_t hit[4]; // Hitbox, index 0 is x offset, 1 is y offset, 2 is width, 3 is height
	
	// momentum variables
	subpixel_t momx;
	subpixel_t momy;
	
	// animation variables
	int anim_state; // animation state, standing, walking, etc
	int anim_timer; // animation timer, value depends on the animation and situation

	// player reference pointer
	struct player_s *player;
} object_t;

extern object_t objects;

// Including the camera in the object code out of laziness?
typedef struct camera_s
{
	// Camera mode, will be defined laterz
	uint8_t mode;
	
	// X and Y coordinates
	subpixel_t x;
	subpixel_t y;
	
	// Optional target object for certain modes
	object_t target;
} camera_t;

extern camera_t camera;

typedef enum
{
	OBJ_NULL,
	OBJ_MARIL // ts bitch playable or something
} objecttypes_e;

void OBJ_InitObjects(void);
void OBJ_RunObjects(void);
object_t *OBJ_CreateObject(subpixel_t x, subpixel_t y, int type);
void OBJ_RemoveObject(object_t *obj);

#endif
