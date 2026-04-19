// JADEFRACTURE
// game_gfx.c

#include "game_gfx.h"
#include "i_system.h"

gfx_t GFX_LoadGFX(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	gfx_t gfx;
	
	// check file
	if (!file)
	{
		printf("Failed to open %s!\n", filename);
	}
	
	// allocate memory for gfx
	memset(&gfx, 0, sizeof(gfx_t));
	
	// read gfx header		
	fread(&gfx.size, sizeof(uint16_t), 1, file);
	gfx.size++;
	
	fread(&gfx.width, sizeof(uint8_t), 1, file);
	gfx.width++;

	fread(&gfx.xoff, sizeof(int8_t), 1, file);
	fread(&gfx.yoff, sizeof(int8_t), 1, file);
	
	// allocate memory for pixel data
	gfx.data = malloc(gfx.size);
	memset(gfx.data, 0, gfx.size);
	
	for (int p = 0; p < gfx.size; p++)
	{
		fread(&gfx.data[p], sizeof(uint8_t), 1, file);
	}
	
	//I_Error("GFX Info Test:\nSize: %d\nWidth: %d\nX offset: %d\nY offset: %d\n", gfx.size, gfx.width, gfx.xoff, gfx.yoff);
	
	fclose(file);
	return gfx;
}