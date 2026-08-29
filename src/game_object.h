// Nozomi Engine
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"
#include "game_video.h"

typedef struct object_s
{
	// base variables
	uint32_t type; // Object type
	struct object_s *prev; // Previous object
	struct object_s *next; // Next object
	uint32_t x; // X position relative to the world
	uint32_t y; // Y position relative to the world
	uint8_t dir; // Facing direction 0 is down, 3 is right
	uint32_t flags; // Any special flags for this object.
	uint32_t hit[4]; // Hitbox, index 0 is x offset, 1 is y offset, 2 is width, 3 is height
	uint8_t layer;
	
	// momentum variables
	int32_t momx;
	int32_t momy;
	
	// animation variables
	uint32_t anim_state; // animation state, standing, walking, etc
	uint32_t anim_timer; // animation timer, value depends on the animation and situation

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
	uint32_t x;
	uint32_t y;
	
	// Optional target object for certain modes
	object_t *target;
} camera_t;

extern camera_t camera;

void OBJ_InitObjects(void);
void OBJ_RunObjects(void);
void OBJ_FreeObjects(void);
object_t *OBJ_CreateObject(uint32_t x, uint32_t y, int type);
void OBJ_RemoveObject(object_t *obj);
void OBJ_DrawObjectLayer(uint8_t layer);
bool OBJ_TryMovement(object_t *obj, int32_t x, int32_t y);

typedef enum
{
	OBJ_NULL, // you can set an object's type to this to remove it :D ... or use OBJ_RemoveObject
	NUMOBJTYPES
} objecttypes_e;

#endif
