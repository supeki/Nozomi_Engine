// Nozomi Engine
// game_object.c

#include "i_system.h"

#include "game_dialogue.h"
#include "game_object.h"
#include "game_player.h"
#include "game_world.h"

object_t objects;
object_info_t *object_info; // stores object information for every object
uint32_t num_object_info = NUM_DEF_OBJECTS;

// The camera is its own type, but we'll include it here ya
camera_t camera;

void OBJ_InitObjects(void)
{
	objects.prev = objects.next = &objects;
	object_info = malloc(num_object_info * sizeof(object_info_t));

	// set OBJ_NULL and OBJ_PLAYER defs below
	// these object types are hardcoded into the engine for important reasons

}

void OBJ_RunObjects(void)
{
	object_t *obj = objects.next;
	
	while (obj != &objects)
	{
		switch (obj->type)
		{				
			case OBJ_NULL:
				break;
		}
		
		if (obj->type == OBJ_NULL) {
			object_t *obj2 = obj->next;
			OBJ_RemoveObject(obj);
			obj = obj2;
			continue;
		}
		
		obj = obj->next;
	}
}

void OBJ_FreeObjects(void)
{
	object_t *obj = objects.next;
	
	while (obj != &objects)
	{	
		object_t *obj2 = obj->next;
		OBJ_RemoveObject(obj);
		obj = obj2;
	}
}

object_t *OBJ_CreateObject(uint16_t x, uint16_t y, uint16_t type)
{
	object_t *obj;
	int i;

	obj = malloc(sizeof(object_t));
	memset(obj, 0, sizeof(object_t));
	
	obj->type = type;
	obj->x = x;
	obj->y = y;

	// object info
	obj->info = &object_info[type];
	obj->health = obj->info->health;
	obj->flags = obj->info->flags;
	for (i = 0; i < 4; i++)
		obj->hit[i] = obj->info->hit[i];
	
	// add this object to the list of objects
	objects.prev->next = obj;
	obj->next = &objects;
	obj->prev = objects.prev;
	objects.prev = obj;

	return obj;
}

void OBJ_RemoveObject(object_t *obj)
{
	obj->next->prev = obj->prev;
	obj->prev->next = obj->next;
	free(obj);
}

void OBJ_DrawObjectLayer(uint8_t layer)
{
	object_t *obj = objects.next;
	
	while (obj != &objects)
	{		
		obj = obj->next;
	}
}

// x and y are by tile
static bool OBJ_InTile(object_t *obj, uint16_t x, uint16_t y)
{
	
}

static bool OBJ_InObject(object_t *obj, object_t *obj2)
{
	
}

bool OBJ_TryMovement(object_t *obj, int8_t x, int8_t y)
{	
	
}

// Object Info

const char *anim_names[NUM_ANIMS] = {
	"Stand",
	"Idle",
	"Move",
	"Push",
	"Swim",
	"Swim Move",
	"Jump",
	"Attack",
	"Item Get",
	"Fall",
	"Dead"
};

void OBJINFO_CreateNewObjectAnimFrame(object_info_t obj_info, uint32_t anim_num) 
{
	object_animframe_t *temp;

	obj_info.anims[anim_num].num_frames++;
	temp = realloc(
		obj_info.anims[anim_num].frames, 
		obj_info.anims[anim_num].num_frames * sizeof(object_animframe_t)
	);

	if (temp == NULL) {
		I_printf(
			"Cannot allocate frame %d for object %d animation %s!\n", 
			obj_info.anims[anim_num].num_frames,
			obj_info.id,
			anim_names[anim_num]
		);

		obj_info.anims[anim_num].num_frames--;
		return;
	}

	I_printf(
		"Allocated frame %d for object %d animation %s.\n", 
		obj_info.anims[anim_num].num_frames,
		obj_info.id,
		anim_names[anim_num]
	);
	
	obj_info.anims[anim_num].frames = temp;
}

void OBJINFO_CreateNewObjectInfo(void) 
{
	object_info_t *temp;
	int i;

	num_object_info++;
	temp = realloc(object_info, num_object_info * sizeof(object_info_t));

	if (temp == NULL) {
		I_printf("Cannot allocate memory for object %d!\n", num_object_info);
		num_object_info--;
		return;
	}

	object_info = temp;

	// give each object an id (mainly for debugging lol)
	object_info[num_object_info-1].id = num_object_info-1;

	// other info
	object_info[num_object_info-1].health = 1; // one hp
	object_info[num_object_info-1].flags = 0; // no flags

	// no hitbox
	object_info[num_object_info-1].hit[0] = 0;
	object_info[num_object_info-1].hit[1] = 0;
	object_info[num_object_info-1].hit[2] = 0;
	object_info[num_object_info-1].hit[3] = 0;

	// no logic
	object_info[num_object_info-1].spawn_logic = 0;
	object_info[num_object_info-1].active_logic = 0;
	object_info[num_object_info-1].death_logic = 0;

	// default to one frame per anim
	for (i = 0; i < NUM_ANIMS; i++) {
		object_info[num_object_info-1].anims[i].frames = NULL; // set to NULL first just in case so realloc doesn't break
		OBJINFO_CreateNewObjectAnimFrame(object_info[num_object_info-1], i);
	}
}

bool objectinfo_edit = false;
dirfiles_t object_dirfiles;
static bool file_menu = false;
static char temp_objectinfo_name[33];

void OBJINFO_LoadObjectInfoFile(const char *filename)
{
	
}

void OBJINFO_StartObjectInfoEdit(const char *filename)
{
    if (filename == NULL)
    {
        objectinfo_edit = true;
        file_menu = true;
        DF_UpdateDirfiles(&object_dirfiles, va("%sdata/", I_GetHomeDir()));
        return;
    }

	OBJINFO_LoadObjectInfoFile(va("%sdata/%s.inf", I_GetHomeDir(), filename));
	sprintf(temp_objectinfo_name, filename);

    file_menu = false;
    objectinfo_edit = true;
}
