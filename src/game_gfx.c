// Nozomi Engine
// game_gfx.c

#include "game_defs.h"
#include "game_gfx.h"
#include "game_video.h"
#include "i_system.h"
#include "helpers/bitmap.h"

gfx_t gfx_her, gfx_tiles, gfx_tree, gfx_tree2, gfx_tree3, gfx_textbox;

void GFX_InitGFX(void)
{
	gfx_her = GFX_LoadGFX("data/her.bmp");
	gfx_tiles = GFX_LoadGFX("data/tiles.bmp");
	gfx_tree = GFX_LoadGFX("data/tree1.bmp");
	gfx_tree2 = GFX_LoadGFX("data/tree2.bmp");
	gfx_tree3 = GFX_LoadGFX("data/tree3.bmp");
	gfx_textbox = GFX_LoadGFX("data/box.bmp");
}

// Load a Bitmap and convert it to our GFX format
gfx_t GFX_LoadGFX(const char *filename)
{
	gfx_t gfx;
	int x, y;
	bitmap_t bitmap = Bitmap_Load(filename);
	
	gfx.width = bitmap.width;
	gfx.height = bitmap.height;
	gfx.data = malloc(bitmap.width * bitmap.height * sizeof(uint16_t));

	switch (bitmap.bpp) 
	{
		case 4:
			for (y = 0; y < bitmap.height; y++)
				for (x = 0; x < bitmap.width; x+=2)
				{
					uint32_t index = y * bitmap.width + x;
					rgb_t pal = bitmap.palette[bitmap.pixel_data[index>>1] >> 4];
					rgb_t pal2 = bitmap.palette[bitmap.pixel_data[index>>1] & 0xF];

					gfx.data[(bitmap.height - (y+1)) * bitmap.width + x] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + (x+1)] = ((pal2.r >> 3) << 11) | ((pal2.g >> 2) << 5) | (pal2.b >> 3);
				}
			break;
		case 8:
			for (y = 0; y < bitmap.height; y++)
				for (x = 0; x < bitmap.width; x++)
				{
					uint32_t index = y * bitmap.width + x;
					rgb_t pal = bitmap.palette[bitmap.pixel_data[index]];
					
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + x] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
				}
			break;
		case 16:
			for (y = 0; y < bitmap.height; y++)
				for (x = 0; x < bitmap.width*2; x+=2)
				{
					uint32_t index = y * (bitmap.width*2) + x;
					uint16_t color = bitmap.pixel_data[index]|(bitmap.pixel_data[index+1]<<8);
					rgb_t pal;
					
					pal.r = (color >> 10) & 0x1F;
					pal.g = (color >> 5) & 0x1F;
					pal.b = color & 0x1F;
					
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + (x/2)] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
				}
			break;
		case 24:
			for (y = 0; y < bitmap.height; y++)
				for (x = 0; x < bitmap.width*3; x+=3)
				{
					uint32_t index = y * (bitmap.width*3) + x;
					rgb_t pal;
					
					pal.b = bitmap.pixel_data[index];
					pal.g = bitmap.pixel_data[index+1];
					pal.r = bitmap.pixel_data[index+2];
					
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + (x/3)] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
				}
			break;
		default:
			I_printf("Unsupported Bitmap BitsPerPixel!\n");
			return gfx;
	}

	Bitmap_Free(&bitmap);

	return gfx;
}

void GFX_FreeGFX(gfx_t *gfx)
{
	gfx->width = 0;
	gfx->height = 0;

	if (gfx->data != NULL) {
		free(gfx->data);
		gfx->data = NULL;
	}
}

gfx_t GFX_LoadLegacyGFX(const char *filename)
{
	gfx_t gfx;
	FILE *file = fopen(filename, "rb");
	int p;
	uint32_t size;

	// check file
	if (!file)
		I_Error("Failed to open %s!\n", filename);
	
	// allocate memory for gfx
	memset(&gfx, 0, sizeof(gfx_t));
	
	// read gfx header		
	fread(&size, sizeof(uint32_t), 1, file);
	size++;
	
	fread(&gfx.width, sizeof(uint16_t), 1, file);
	gfx.width++;
	gfx.height = size / gfx.width;
	
	// allocate memory for pixel data
	gfx.data = malloc(size);
	memset(gfx.data, 0, size);
	
	for (p = 0; p < size; p++) {
		uint8_t index;
		fread(&index, sizeof(uint8_t), 1, file);
		gfx.data[p] = palette[index];
	}
	
	fclose(file);
	return gfx;
}