// JADEFRACTURE
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"
#include "game_video.h"

typedef struct state_s
{
	uint16_t sprite;
	uint16_t frame; // starting frame
	uint16_t numframes; // number of frames
	uint16_t fps; // number of sprite frames to cycle through per second
	uint32_t statetime; // how many frames this state lasts for 
	uint32_t next; // number for next state to go to
} state_t;

typedef struct object_s
{
	// base variables
	uint32_t type; // Object type
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
	subpixel_t x;
	subpixel_t y;
	
	// Optional target object for certain modes
	object_t *target;
} camera_t;

extern camera_t camera;

void OBJ_InitObjects(void);
void OBJ_RunObjects(void);
object_t *OBJ_CreateObject(subpixel_t x, subpixel_t y, int type);
void OBJ_RemoveObject(object_t *obj);

typedef enum
{
	OBJ_NULL, // you can set an object's type to this to remove it :D ... or use OBJ_RemoveObject
	OBJ_MARIL, // ts bitch playable or something
	NUMOBJTYPES
} objecttypes_e;

typedef enum
{
	S_NULL,
	S_MARIL_STAND, // she stand
	S_MARIL_WALK, // she walcc
	S_MARIL_WIN, // she maybe even do a little pose
	NUMSTATES
} states_e;

#endif
