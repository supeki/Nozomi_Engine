// Nozomi Engine
// game_object.c

#include "game_dialogue.h"
#include "game_object.h"
#include "game_player.h"
#include "game_world.h"

object_t objects;

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

object_t *OBJ_CreateObject(uint32_t x, uint32_t y, int type)
{
	object_t *obj;
	obj = malloc(sizeof(object_t));
	memset(obj, 0, sizeof(object_t));
	
	obj->type = type;
	obj->x = x;
	obj->y = y;
	
	switch (obj->type) {
		default:
			obj->hit[0] = 0;
			obj->hit[1] = 0;
			obj->hit[2] = 16*PU;
			obj->hit[3] = 16*PU;
			break;
	}
	
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

static int walk_table[4] = {1, 0, 2, 0};

void OBJ_DrawObjectLayer(uint8_t layer)
{
	object_t *obj = objects.next;
	
	while (obj != &objects)
	{		
		obj = obj->next;
	}
}

static bool OBJ_InTile(object_t *obj, uint32_t x, uint32_t y)
{
	uint32_t hit_left = obj->x + obj->hit[0];
	uint32_t hit_top = obj->y + obj->hit[1];
	uint32_t hit_right = hit_left + obj->hit[2];
	uint32_t hit_bottom = hit_top + obj->hit[3];
	uint32_t hit2_left = x;
	uint32_t hit2_top = y;
	uint32_t hit2_right = x + 16*PU;
	uint32_t hit2_bottom = y + 16*PU;
	
	return (
		hit_left <= hit2_right - 1 &&
		hit_right - 1 >= hit2_left &&
		hit_bottom - 1 >= hit2_top &&
		hit_top <= hit2_bottom - 1
	);
}

static bool OBJ_InObject(object_t *obj, object_t *obj2)
{
	uint32_t hit_left = obj->x + obj->hit[0];
	uint32_t hit_top = obj->y + obj->hit[1];
	uint32_t hit_right = hit_left + obj->hit[2];
	uint32_t hit_bottom = hit_top + obj->hit[3];
	uint32_t hit2_left = obj2->x + obj2->hit[0];
	uint32_t hit2_top = obj2->y + obj2->hit[1];
	uint32_t hit2_right = hit2_left + obj2->hit[2];
	uint32_t hit2_bottom = hit2_top + obj2->hit[3];
	
	return (
		hit_left <= hit2_right - 1 &&
		hit_right - 1 >= hit2_left &&
		hit_bottom - 1 >= hit2_top &&
		hit_top <= hit2_bottom - 1
	);
}

bool OBJ_TryMovement(object_t *obj, int32_t x, int32_t y)
{	
	int32_t step_x = (x > 0 ? 1 : -1)*PU, step_y = (y > 0 ? 1 : -1)*PU;
	int32_t x2, y2;

	if (x == 0 && y == 0)
		return false;
	
	if (x != 0)
		for (x2 = 0; x2 < abs(x/PU); x2++)
		{
			uint32_t hit_x, hit_y, hit_x2, hit_y2;
			object_t *obj2;
			int i, i2, i3, i4, res;

			obj->x += step_x;
			hit_x = obj->x + obj->hit[0];
			hit_y = obj->y + obj->hit[1];
			hit_x2 = hit_x + obj->hit[2];
			hit_y2 = hit_y + obj->hit[3];
			
			obj2 = objects.next;
			while (obj2 != &objects)
			{
				if (OBJ_InObject(obj, obj2))
				{
					// do object collision ig
				}			
				
				obj2 = obj2->next;
			}
			
			if (x == 0)
				break;
		}
		
	if (y != 0)
		for (y2 = 0; y2 < abs(y/PU); y2++)
		{
			uint32_t hit_x, hit_y, hit_x2, hit_y2;
			object_t *obj2;
			int i, i2, i3, i4, res;

			obj->y += step_y;
			hit_x = obj->x + obj->hit[0];
			hit_y = obj->y + obj->hit[1];
			hit_x2 = hit_x + obj->hit[2];
			hit_y2 = hit_y + obj->hit[3];
			
			obj2 = objects.next;
			while (obj2 != &objects)
			{
				if (OBJ_InObject(obj, obj2))
				{
					// do object collision ig
				}	
				
				obj2 = obj2->next;
			}
			
			if (y == 0)
				break;
		}
	
	return (x != 0 || y != 0);
}
