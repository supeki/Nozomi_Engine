// Nozomi Engine
// game_object.h

#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "game_defs.h"
#include "game_video.h"

typedef enum
{
	ANIM_STAND, // standing
	ANIM_IDLE, // idle
	ANIM_MOVE, // moving
	ANIM_PUSH, // pushing against something
	ANIM_SWIM, // swimmers
	ANIM_SWIM_MOVE, // the swimmers but you move or something
	ANIM_JUMP, // jumping
	ANIM_ATTACK, // attacking
	ANIM_ITEMGET, // item getting
	ANIM_OHDEARGODIMFALLING, // falling into a pit
	ANIM_DEAD, // you died
	NUM_ANIMS 
} obj_anims_e;

extern const char *anim_names[NUM_ANIMS];

typedef struct 
{
	uint8_t x_off;
	uint8_t y_off;
	uint8_t width;
	uint8_t height;
} object_animframe_t;

typedef struct
{
	uint8_t fps;
	uint8_t num_frames;
	uint8_t dir_type;

	object_animframe_t *frames;
} object_animtype_t;

typedef struct object_info_s
{
	uint32_t id;
	uint8_t health;
	uint32_t flags;
	uint8_t hit[4];

	uint32_t spawn_logic;
	uint32_t active_logic;
	uint32_t death_logic;

	object_animtype_t anims[NUM_ANIMS];
} object_info_t;

typedef struct object_s
{
	// base variables
	uint16_t type; // obj type
	struct object_s *prev; // previous obj
	struct object_s *next; // next obj

	// object info reference pointer
	struct object_info_s *info;

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
	
	obj_anims_e anim_state; // obj anim state
	uint16_t anim_timer; // current anim tick

	// player reference pointer - player objs
	struct player_s *player;
} object_t;

typedef enum
{
	OBJ_NULL, 
	OBJ_PLAYER,
	NUM_DEF_OBJECTS
} object_types_e; // these two object types already exist for pretty obvious reasons

extern object_t objects;
extern object_info_t *object_info;
extern uint32_t num_object_info;

// Including the camera in the object code out of laziness?
typedef struct camera_s
{
	// Camera mode, will be defined laterz
	uint8_t mode;
	
	// X and Y coordinates
	int32_t x;
	int32_t y;
	
	// Optional target object for certain modes
	object_t *target;
} camera_t;

extern camera_t camera;

void OBJ_InitObjects(void);
void OBJ_RunObjects(void);
void OBJ_FreeObjects(void);
object_t *OBJ_CreateObject(uint16_t x, uint16_t y, uint16_t type);
void OBJ_RemoveObject(object_t *obj);
void OBJ_DrawObjectLayer(uint8_t layer);
bool OBJ_TryMovement(object_t *obj, int8_t x, int8_t y);

void OBJINFO_CreateNewObjectAnimFrame(object_info_t obj_info, uint32_t anim_num);
void OBJINFO_CreateNewObjectInfo(void);

extern bool objectinfo_edit;
void OBJINFO_LoadObjectInfoFile(const char *filename);
void OBJINFO_StartObjectInfoEdit(const char *filename);

#endif
