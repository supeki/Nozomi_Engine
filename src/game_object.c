// JADEFRACTURE
// game_object.c

#include "game_object.h"
#include "game_player.h"

object_t objects;

void OBJ_InitObjects(void)
{
	int i;
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
			case OBJ_MARIL:
				PlayerLogic(obj);
				break;
		}
		
		if (obj->type == OBJ_NULL)
			OBJ_RemoveObject(obj);
		
		obj = obj->next;
	}
}

object_t *OBJ_CreateObject(subpixel_t x, subpixel_t y, int type)
{
	object_t *obj;
	obj = malloc(sizeof(object_t));
	memset(obj, 0, sizeof(object_t));
	
	obj->type = type;
	obj->x = x;
	obj->y = y;
	
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