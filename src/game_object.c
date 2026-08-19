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
			case OBJ_CHECK:
				obj->type = OBJ_NULL; // kill yourself!!!!
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

object_t *OBJ_CreateObject(subpixel_t x, subpixel_t y, int type)
{
	object_t *obj;
	obj = malloc(sizeof(object_t));
	memset(obj, 0, sizeof(object_t));
	
	obj->type = type;
	obj->x = x;
	obj->y = y;
	
	switch (obj->type) {
		case OBJ_MARIL:
			obj->hit[0] = 8*PU; // x pos
			obj->hit[1] = 16*PU; // y pos
			obj->hit[2] = 8*PU; // width
			obj->hit[3] = 16*PU; // height
			break;
		case OBJ_CHECK:
			obj->hit[0] = 4*PU;
			obj->hit[1] = 4*PU;
			obj->hit[2] = 8*PU;
			obj->hit[3] = 8*PU;
			break;
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
		if (obj->type == OBJ_MARIL)
		{
			if ((obj->y + obj->hit[1] + obj->hit[3])/PU >= 96)
				V_Draw(gfx_tree, 128 - gfx_tree.width/2, 96 - gfx_tree.height - camera.y, 0);
			
			if (obj->anim_timer > 0)
				V_DrawCropped(gfx_her, obj->x/PU, obj->y/PU - camera.y, walk_table[(obj->anim_timer/6) % 4]*24, obj->dir*32, 24, 32, 0);
			else
				V_DrawCropped(gfx_her, obj->x/PU, obj->y/PU - camera.y, 0, obj->dir*32, 24, 32, 0);
			
			if ((obj->y + obj->hit[1] + obj->hit[3])/PU < 96)
				V_Draw(gfx_tree, 128 - gfx_tree.width/2, 96 - gfx_tree.height - camera.y, 0);
		}
		
		obj = obj->next;
	}
}

static bool OBJ_InTile(object_t *obj, subpixel_t x, subpixel_t y)
{
	subpixel_t hit_left = obj->x + obj->hit[0];
	subpixel_t hit_top = obj->y + obj->hit[1];
	subpixel_t hit_right = hit_left + obj->hit[2];
	subpixel_t hit_bottom = hit_top + obj->hit[3];
	subpixel_t hit2_left = x;
	subpixel_t hit2_top = y;
	subpixel_t hit2_right = x + 16*PU;
	subpixel_t hit2_bottom = y + 16*PU;
	
	return (
		hit_left <= hit2_right - 1 &&
		hit_right - 1 >= hit2_left &&
		hit_bottom - 1 >= hit2_top &&
		hit_top <= hit2_bottom - 1
	);
}

static bool OBJ_InObject(object_t *obj, object_t *obj2)
{
	subpixel_t hit_left = obj->x + obj->hit[0];
	subpixel_t hit_top = obj->y + obj->hit[1];
	subpixel_t hit_right = hit_left + obj->hit[2];
	subpixel_t hit_bottom = hit_top + obj->hit[3];
	subpixel_t hit2_left = obj2->x + obj2->hit[0];
	subpixel_t hit2_top = obj2->y + obj2->hit[1];
	subpixel_t hit2_right = hit2_left + obj2->hit[2];
	subpixel_t hit2_bottom = hit2_top + obj2->hit[3];
	
	return (
		hit_left <= hit2_right - 1 &&
		hit_right - 1 >= hit2_left &&
		hit_bottom - 1 >= hit2_top &&
		hit_top <= hit2_bottom - 1
	);
}

bool OBJ_TryMovement(object_t *obj, subpixel_t x, subpixel_t y)
{	
	subpixel_t step_x = (x > 0 ? 1 : -1)*PU, step_y = (y > 0 ? 1 : -1)*PU;
	uint32_t x2, y2;

	if (x == 0 && y == 0)
		return false;
	
	if (x != 0)
		for (x2 = 0; x2 < abs(x/PU); x2++)
		{
			subpixel_t hit_x, hit_y, hit_x2, hit_y2;
			object_t *obj2;
			int i, i2, i3, i4, res;
			obj->x += step_x;
			
			hit_x = obj->x + obj->hit[0];
			hit_y = obj->y + obj->hit[1];
			hit_x2 = hit_x + obj->hit[2];
			hit_y2 = hit_y + obj->hit[3];
			
			if ( // hardcoded tree,,,,
				hit_x2 - 1 >= 112*PU &&
				hit_x <= 144*PU - 1 &&
				hit_y2 - 1 >= 80*PU &&
				hit_y <= 88*PU - 1
			) {
				obj->x -= step_x;
				x = 0;
				break;
			}
			
			obj2 = objects.next;
	
			while (obj2 != &objects)
			{
				if (OBJ_InObject(obj, obj2))
					if (obj2->type == OBJ_MAN){
						obj->x -= step_x;
						x = 0;
						
						if (obj->type == OBJ_CHECK) {
							D_StartDialogue(obj2->anim_state);
							
							if (obj2->anim_state == 0)
								obj2->anim_state = 4;
							else if (obj2->anim_state == 4)
								obj2->anim_state = 6;
						}
						
						break;
					}
						
				
				obj2 = obj2->next;
			}
			
			if (x == 0)
				break;
			
			i = obj->x/PU/16 + (obj->y/PU/16)*16 + 16;
			i2 = obj->x/PU/16 + (obj->y/PU/16)*16 + 17;
			i3 = obj->x/PU/16 + (obj->y/PU/16)*16 + 32;
			i4 = obj->x/PU/16 + (obj->y/PU/16)*16 + 33;
			res = 0;
			
			if (OBJ_InTile(obj, (i%16)*(16*PU), (i/16)*(16*PU)))
				res+=1;
			if (OBJ_InTile(obj, (i2%16)*(16*PU), (i2/16)*(16*PU)))
				res+=2;
			if (OBJ_InTile(obj, (i3%16)*(16*PU), (i3/16)*(16*PU)))
				res+=4;
			if (OBJ_InTile(obj, (i4%16)*(16*PU), (i4/16)*(16*PU)))
				res+=8;
			
			if (res > 0) {
				if (res & 1)
					if (demo_tiles[i] != 1) {
						obj->x -= step_x;
						x = 0;
						break;
					}
				if (res & 2)
					if (demo_tiles[i2] != 1) {
						obj->x -= step_x;
						x = 0;
						break;
					}
				if (res & 4)
					if (demo_tiles[i3] != 1) {
						obj->x -= step_x;
						x = 0;
						break;
					}
				if (res & 8)
					if (demo_tiles[i4] != 1) {
						obj->x -= step_x;
						x = 0;
						break;
					}
			}
		}
		
	if (y != 0)
		for (y2 = 0; y2 < abs(y/PU); y2++)
		{
			subpixel_t hit_x, hit_y, hit_x2, hit_y2;
			object_t *obj2;
			int i, i2, i3, i4, res;
			obj->y += step_y;
			
			hit_x = obj->x + obj->hit[0];
			hit_y = obj->y + obj->hit[1];
			hit_x2 = hit_x + obj->hit[2];
			hit_y2 = hit_y + obj->hit[3];
			
			if ( // hardcoded tree,,,,
				hit_x2 - 1 >= 112*PU &&
				hit_x <= 144*PU - 1 &&
				hit_y2 - 1 >= 80*PU &&
				hit_y <= 88*PU - 1
			) {
				obj->y -= step_y;
				y = 0;
				break;
			}
			
			obj2 = objects.next;
	
			while (obj2 != &objects)
			{
				if (OBJ_InObject(obj, obj2))
					if (obj2->type == OBJ_MAN){
						obj->y -= step_y;
						y = 0;
						
						if (obj->type == OBJ_CHECK) {
							D_StartDialogue(obj2->anim_state);
							
							if (obj2->anim_state == 0)
								obj2->anim_state = 4;
							else if (obj2->anim_state == 4)
								obj2->anim_state = 6;
						}
						
						break;
					}
						
				
				obj2 = obj2->next;
			}
			
			if (y == 0)
				break;
			
			i = obj->x/PU/16 + (obj->y/PU/16)*16 + 16;
			i2 = obj->x/PU/16 + (obj->y/PU/16)*16 + 17;
			i3 = obj->x/PU/16 + (obj->y/PU/16)*16 + 32;
			i4 = obj->x/PU/16 + (obj->y/PU/16)*16 + 33;
			res = 0;
			
			if (OBJ_InTile(obj, (i%16)*(16*PU), (i/16)*(16*PU)))
				res+=1;
			if (OBJ_InTile(obj, (i2%16)*(16*PU), (i2/16)*(16*PU)))
				res+=2;
			if (OBJ_InTile(obj, (i3%16)*(16*PU), (i3/16)*(16*PU)))
				res+=4;
			if (OBJ_InTile(obj, (i4%16)*(16*PU), (i4/16)*(16*PU)))
				res+=8;
			
			if (res > 0) {
				if (res & 1)
					if (demo_tiles[i] != 1) {
						obj->y -= step_y;
						y = 0;
						break;
					}
				if (res & 2)
					if (demo_tiles[i2] != 1) {
						obj->y -= step_y;
						y = 0;
						break;
					}
				if (res & 4)
					if (demo_tiles[i3] != 1) {
						obj->y -= step_y;
						y = 0;
						break;
					}
				if (res & 8)
					if (demo_tiles[i4] != 1) {
						obj->y -= step_y;
						y = 0;
						break;
					}
			}
		}
	
	return (x != 0 || y != 0);
}
