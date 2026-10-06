// Nozomi Engine
// game_object.c

#include "i_system.h"

#include "game_dialogue.h"
#include "game_font.h"
#include "game_input.h"
#include "game_object.h"
#include "game_player.h"
#include "game_world.h"

object_t objects;
object_info_t *object_info; // stores object information for every object
uint32_t num_object_info = 0;

gfx_t *object_gfx; // table to store currently loaded gfx that each object calls
char **object_gfx_names; // it's like some kind of.. table of gfx names..
uint32_t num_object_gfx = 0;

// The camera is its own type, but we'll include it here ya
camera_t camera;

void OBJ_InitObjects(void)
{
	objects.prev = objects.next = &objects;
	OBJINFO_DefaultObjectInfo(); // default objs + info
	OBJINFO_DefaultObjectGFX(); // dummy gfx
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

// add a graphic name should it not be in our list already
// also loads the graphic
// returns an ID right away for quick usage, if it can't reallocate, return 0 for dummy gfx
uint32_t OBJINFO_AddObjectGFX(const char *gfx_name)
{
	char **temp_names;
	gfx_t *temp_gfx;

	// hardcoded exception
	if (!strcmp(gfx_name, "dummy") && num_object_gfx > 0)
		return 0;

	// make a larger array
	num_object_gfx++;
	temp_names = realloc(object_gfx_names, num_object_gfx * sizeof(char *));

	// couldn't allocate
	if (temp_names == NULL) {
		num_object_gfx--;
		return 0;
	}

	strcpy(temp_names[num_object_gfx-1], gfx_name);
	object_gfx_names = temp_names;

	temp_gfx = realloc(object_gfx, num_object_gfx * sizeof(gfx_t));

	if (temp_gfx == NULL) {
		num_object_gfx--;
		return 0;
	}

	temp_gfx[num_object_gfx-1] = GFX_LoadGFX(va("%sdata/objects/%s.bmp", I_GetHomeDir(), gfx_name));
	if (temp_gfx[num_object_gfx-1].width * temp_gfx[num_object_gfx-1].height <= 0) {
		num_object_gfx--;
		return 0;
	}

	object_gfx = temp_gfx;
	return num_object_gfx-1;
}

// check if a graphic is already loaded by referring to our list
// if it exists, give its number, if it doesn't, return 0 (dummy gfx)
uint32_t OBJINFO_GetObjectGFXNameID(const char *gfx_name)
{
	int i;

	for (i = 0; i < num_object_gfx; i++)
		if (!strcmp(object_gfx_names[i], gfx_name))
			return i;

	return 0;
}

// frees the names AND the gfx
void OBJINFO_FreeObjectGFX(void)
{
	int i;

	if (num_object_gfx > 0) {
		for (i = 0; i < num_object_gfx; i++) {
			free(object_gfx_names[i]);
			GFX_FreeGFX(&object_gfx[i]);
		}

		free(object_gfx_names);
		object_gfx_names = NULL;

		free(object_gfx);
		object_gfx = NULL;
	}

	num_object_gfx = 0;
}

void OBJINFO_DefaultObjectGFX(void)
{
	OBJINFO_FreeObjectGFX(); // free stuff
	OBJINFO_AddObjectGFX("dummy"); // dummy
}

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

void OBJINFO_CreateNewObjectAnimFrame(object_info_t *obj_info, uint32_t anim_num) 
{
	object_animframe_t *temp;

	obj_info->anims[anim_num].num_frames++;
	temp = realloc(
		obj_info->anims[anim_num].frames, 
		obj_info->anims[anim_num].num_frames * sizeof(object_animframe_t)
	);

	if (temp == NULL) {
		I_printf(
			"Cannot allocate frame %d for object %d animation %s!\n", 
			obj_info->anims[anim_num].num_frames,
			obj_info->id,
			anim_names[anim_num]
		);

		obj_info->anims[anim_num].num_frames--;
		return;
	}

	/*
	I_printf(
		"Allocated frame %d for object %d animation %s.\n", 
		obj_info->anims[anim_num].num_frames,
		obj_info->id,
		anim_names[anim_num]
	);
	*/

	memset(
		&temp[obj_info->anims[anim_num].num_frames - 1],
		0,
		sizeof(object_animframe_t)
	);
	
	obj_info->anims[anim_num].frames = temp;
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

	memset(&object_info[num_object_info-1], 0, sizeof(object_info_t));

	// give each object an id (mainly for debugging lol)
	object_info[num_object_info-1].id = num_object_info-1;
	object_info[num_object_info-1].gfx_id = OBJINFO_GetObjectGFXNameID("dummy");

	// other info
	sprintf(object_info[num_object_info-1].name, va("Object %d", num_object_info-NUM_DEF_OBJECTS));
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
		object_info[num_object_info-1].anims[i].fps = 1;
		object_info[num_object_info-1].anims[i].num_frames = 0;
		object_info[num_object_info-1].anims[i].frames = NULL; // set to NULL first just in case so realloc doesn't break
		OBJINFO_CreateNewObjectAnimFrame(&object_info[num_object_info-1], i);
	}
}

// this function only needs to exist here tbh
static void OBJINFO_FreeObjectInfo(void)
{
	uint32_t i, j;

	if (object_info == NULL)
		return;

	for (i = 0; i < num_object_info; i++)
		for (j = 0; j < NUM_ANIMS; j++)
			if (object_info[i].anims[j].frames != NULL) {
				free(object_info[i].anims[j].frames);
				object_info[i].anims[j].frames = NULL;
				object_info[i].anims[j].num_frames = 0;
			}

	free(object_info);
	object_info = NULL;
	num_object_info = 0;
}

void OBJINFO_DefaultObjectInfo(void)
{
	uint32_t i;
	OBJINFO_FreeObjectInfo();
	OBJINFO_DefaultObjectGFX();

	for (i = 0; i < NUM_DEF_OBJECTS; i++) {
		OBJINFO_CreateNewObjectInfo();
		object_info[i].id = i;
	}

	// set OBJ_NULL and OBJ_PLAYER defs below
	// these object types are hardcoded into the engine for important reasons
	// well actually OBJ_NULL doesn't need anything done lmao
	sprintf(object_info[OBJ_NULL].name, "Null"); // THAT IS A LIEEEEEE
	
	sprintf(object_info[OBJ_PLAYER].name, "Player");
	object_info[OBJ_PLAYER].health = 15;
	object_info[OBJ_PLAYER].flags = 0; // i don't have any object flags yet

	// a square hitbox at the bottom center
	object_info[OBJ_PLAYER].hit[0] = 4; // start x
	object_info[OBJ_PLAYER].hit[1] = 8; // start y
	object_info[OBJ_PLAYER].hit[2] = 8; // width
	object_info[OBJ_PLAYER].hit[3] = 8; // height
}

bool objectinfo_edit = false;
dirfiles_t objectinfo_dirfiles;
static bool file_menu = false;
static int16_t file_sel = 0;
static char temp_objectinfo_name[33];
static uint32_t sel_obj = 0;

void OBJINFO_LoadObjectInfoFile(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	uint32_t i, j, num_objs;

	if (!fp)
	{
		I_printf("Invalid object information file (or not found): %s\n", filename);
		OBJINFO_DefaultObjectInfo();
		return;
	}

	// free previous stuffs
	OBJINFO_FreeObjectInfo();

	// endianness omg
	num_objs = FIL_ReadU32(fp);

	// make object info
	for (i = 0; i < num_objs; i++)
		OBJINFO_CreateNewObjectInfo();

	for (i = 0; i < num_object_info; i++) {
		char gfx_name[33];
		uint32_t gfx_id; 

		// set object info
		object_info[i].id = FIL_ReadU32(fp);
		fread(&gfx_name, sizeof(char), 32, fp);
		gfx_name[32] = '\0';

		gfx_id = OBJINFO_GetObjectGFXNameID(gfx_name);
		if (gfx_id == 0)
			object_info[num_object_info-1].gfx_id = OBJINFO_AddObjectGFX(gfx_name);
		else
			object_info[num_object_info-1].gfx_id = gfx_id;

		fread(&object_info[i].health, sizeof(uint8_t), 1, fp);
		object_info[i].flags = FIL_ReadU32(fp);
		fread(&object_info[i].hit, sizeof(uint8_t), 4, fp);
		object_info[i].spawn_logic = FIL_ReadU32(fp);
		object_info[i].active_logic = FIL_ReadU32(fp);
		object_info[i].death_logic = FIL_ReadU32(fp);

		// set anims
		for (j = 0; j < NUM_ANIMS; j++) {
			uint8_t k;
			fread(&object_info[i].anims[j].fps, sizeof(uint8_t), 1, fp);
			fread(&object_info[i].anims[j].num_frames, sizeof(uint8_t), 1, fp);
			fread(&object_info[i].anims[j].dir_type, sizeof(uint8_t), 1, fp);

			// and make and set their frames
			for (k = 0; k < object_info[i].anims[j].num_frames-1; k++)
				OBJINFO_CreateNewObjectAnimFrame(&object_info[i], j);

			for (k = 0; k < object_info[i].anims[j].num_frames; k++) {
				object_info[i].anims[j].frames[k].x_off = FIL_ReadU16(fp);
				object_info[i].anims[j].frames[k].y_off = FIL_ReadU16(fp);
				object_info[i].anims[j].frames[k].width = FIL_ReadU16(fp);
				object_info[i].anims[j].frames[k].height = FIL_ReadU16(fp);
			}
		}
	}

	fclose(fp);
}

void OBJINFO_StartObjectInfoEdit(const char *filename)
{
    if (filename == NULL)
    {
        objectinfo_edit = true;
        file_menu = true;
		file_sel = 0;
		sel_obj = 0;
        DF_UpdateDirfiles(&objectinfo_dirfiles, va("%sdata/", I_GetHomeDir()));
        return;
    }

	OBJINFO_LoadObjectInfoFile(va("%sdata/%s.inf", I_GetHomeDir(), filename));
	sprintf(temp_objectinfo_name, filename);

	file_sel = 0;
	sel_obj = 0;
    file_menu = false;
    objectinfo_edit = true;
}

void OBJINFO_UpdateObjectInfoEdit(void)
{
	if (file_menu) {
		if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
            file_menu = false;
            return;
        }

		if (G_ControlDown(PLAYER_ONE, CON_SELECT, true)) {
			OBJINFO_DefaultObjectInfo();
			file_sel = 0;
			sel_obj = 0;
			file_menu = false;
			return;
		}

        if (objectinfo_dirfiles.num_files < 1)
            return; // no files so no loading anything

		if (G_ControlDown(PLAYER_ONE, CON_UP, true))
			file_sel--;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
            file_sel++;
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
            file_sel-=16;
        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
            file_sel+=16;

        // sanity checks
        if (file_sel < 0)
            file_sel = objectinfo_dirfiles.num_files - abs(file_sel);

        // still below zero?
        if (file_sel < 0)
            file_sel = 0; // sigh..
        
        // loop around if need be
        file_sel = file_sel % objectinfo_dirfiles.num_files;

		if (G_ControlDown(PLAYER_ONE, CON_A, true)) {
            char *dot;
            char *name;
            const char *filename;
            int name_len;

            filename = objectinfo_dirfiles.filenames[file_sel];
            dot = strrchr(filename, '.');

            name_len = strlen(filename) + 1;
            name = malloc((name_len-4) * sizeof(char)); 
            snprintf(name, name_len-4, "%s", filename);
            name[name_len-4] = '\0'; // fix wince port

            if (!strcmp(dot, ".inf") || !strcmp(dot, ".INF"))
                OBJINFO_StartObjectInfoEdit(name);

            free(name);
        }

		return;
	}

	if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
		file_menu = true;
		return;
	}

	if (num_object_info <= 0)
	    return;

	if (sel_obj == num_object_info) {
		if (G_ControlDown(PLAYER_ONE, CON_A, true))
			OBJINFO_CreateNewObjectInfo();
		if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
			sel_obj = 0; // hacky
	} else if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
		sel_obj++;

	if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
		sel_obj--;
	

	// sanity checks
	if (sel_obj < 0)
		sel_obj = num_object_info;

	// still below zero?
	if (sel_obj < 0)
		sel_obj = 0; // sigh..

	if (sel_obj > num_object_info)
		sel_obj = num_object_info;
	
	// loop around if need be
	sel_obj = sel_obj % (num_object_info+1);
}

void OBJINFO_DrawObjectInfoEdit(void)
{
	int i;
	uint8_t cw = font_default.charsize >> 8;
	uint8_t ch = font_default.charsize & 0xFF;

	if (file_menu) {
        V_DrawText("Nozomi Engine Object Information Editor\nSelect a file to open:", 0, 0, 0);

        if (objectinfo_dirfiles.num_files > 0) {
            for (i = 0; i < objectinfo_dirfiles.num_files; i++) {
				char *dot;
                dot = strrchr(objectinfo_dirfiles.filenames[i], '.');

                if (strcmp(dot, ".inf") && strcmp(dot, ".INF"))
					V_DrawText(va("N/A - %s", objectinfo_dirfiles.filenames[i]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);
				else
					V_DrawText(va("%s", objectinfo_dirfiles.filenames[i]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);
			}

            V_DrawText(">", (file_sel/17) * 80 + 1, (file_sel%17) * 10 + 20, 0);
        } else
            V_DrawText("No files found.", 8, 20, 0);

		V_DrawText("Press Select to create new Object Info. file.", 0, VID_HEIGHT-17, 0);
        V_DrawText("Close the File Menu with Start/Enter", 0, VID_HEIGHT-8, 0);
        // return early
        return;
    }

    if (num_object_info <= 0) {
        V_DrawText("No Object Information exists.\nRe-open the File Menu with Start/Enter.", 0, 0, 0);
        return;
    }

	// draw little object id boxes
	{ // doing this because C sillay
		int i;
		uint16_t x_pos = 8;
		int cur_off = sel_obj - 8;

		if (cur_off < 0)
			cur_off = 0;

		for (i = 0; i < num_object_info; i++) {
			uint16_t id_len;
			int off_set;
			if (i - cur_off < 0)
				continue;

			id_len = strlen(va("%d", object_info[i].id));
			if (i == sel_obj)
				V_DrawBox(x_pos - (id_len*cw)/2, ch/4, 0, (id_len*cw)/2*3, ch/2*3, 0xFFFF, 0);

			V_DrawText(va("%d", object_info[i].id), x_pos - (id_len*cw)/4, ch/2, 0);
			x_pos += cw*(id_len+1);

			off_set = id_len-2;
			if (off_set < 0)
				off_set = 0;
			x_pos -= off_set * cw;

			if (x_pos+8 >= VID_WIDTH)
				break;
		}

		if (i == sel_obj)
			V_DrawBox(x_pos - cw/2, ch/4, 0, cw/2*3, ch/2*3, 0xFFFF, 0);

		if (i == num_object_info)
			V_DrawText("+", x_pos - cw/4, ch/2, 0);
	}

	if (sel_obj < num_object_info)
		V_DrawText(object_info[sel_obj].name, cw, ch*3, 0);
	else
		V_DrawText("New Object", cw, ch*3, 0);

	V_DrawLine(0, ch*4 + ch/4, 90, VID_WIDTH, 0xFFFF, 0);
	V_DrawBox(7, ch*5-1, 0, 66, 66, 0xFFFF, 0); // give 64x64 

	// draw stand sprite here mayb
	if (sel_obj < num_object_info)
	{
		object_animframe_t frame = object_info[sel_obj].anims[ANIM_STAND].frames[0];
		//V_DrawCropped(<put gfx here>, 8, ch*5, frame.x_off, frame.y_off, frame.width, frame.height, 0);
	}

	if (sel_obj < num_object_info)
		V_DrawBox(8+object_info[sel_obj].hit[0], ch*5 + object_info[sel_obj].hit[1], 0, object_info[sel_obj].hit[2], object_info[sel_obj].hit[3], 0x07E0, 0);
}
