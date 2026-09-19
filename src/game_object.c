// Nozomi Engine
// game_object.c

#include "game_dialogue.h"
#include "game_object.h"
#include "game_player.h"
#include "game_world.h"

object_t objects;
object_info_t *object_info; // stores object information for every object

// The camera is its own type, but we'll include it here ya
camera_t camera;

void OBJ_InitObjects(void)
{
	objects.prev = objects.next = &objects;
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

