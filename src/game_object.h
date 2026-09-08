// Nozomi Engine
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"
#include "game_video.h"

typedef struct object_s
{
	// base variables
	uint16_t type; // obj type
	struct object_s *prev; // previous obj
	struct object_s *next; // next obj

	// utility variables
	uint8_t health;
	uint32_t flags; // any special flags for this obj
	uint8_t hit[4]; // hitbox, index 0 is x offset, 1 is y offset, 2 is width, 3 is height

	// positional variables
	uint16_t x; // obj x
	uint16_t y; // obj y
	int8_t momx; // obj momx
	int8_t momy; // obj momy
	uint8_t dir_layer; // LLLLLLDD L - layer D - dir
	
	uint8_t anim_state; // obj anim state
	uint16_t anim_timer; // current anim tick

	// player reference pointer - player objs
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
