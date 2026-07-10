// Nozomi Engine
// game_gfx.c

#include "game_gfx.h"
#include "i_system.h"
#include "helpers/bitmap.h"

gfx_t gfx_characters;
bitmap_gfx_t bmpgfx_characters;

void GFX_InitGFX(void)
{
}

gfx_t GFX_LoadGFX(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	gfx_t gfx;
	
	// check file
	if (!file)
		I_Error("Failed to open %s!\n", filename);
	
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

// Load a Bitmap and convert it to our Bitmap GFX format
bitmap_gfx_t BMPGFX_LoadBitmap(const char *filename)
{
	bitmap_gfx_t gfx;
	bitmap_t bitmap = Bitmap_Load(filename);
	
	gfx.width = bitmap.width;
	gfx.height = bitmap.height;
	gfx.data = malloc(bitmap.width * bitmap.height * sizeof(uint16_t));

	switch (bitmap.bpp) 
	{
		case 4:
			for (int y = 0; y < bitmap.height; y++)
				for (int x = 0; x < bitmap.width; x+=2)
				{
					uint32_t index = y * bitmap.width + x;
					rgb_t pal = bitmap.palette[bitmap.pixel_data[index>>1] >> 4];
					rgb_t pal2 = bitmap.palette[bitmap.pixel_data[index>>1] & 0xF];

					gfx.data[(bitmap.height - (y+1)) * bitmap.width + x] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + (x+1)] = ((pal2.r >> 3) << 11) | ((pal2.g >> 2) << 5) | (pal2.b >> 3);
				}
			break;
		case 8:
			for (int y = 0; y < bitmap.height; y++)
				for (int x = 0; x < bitmap.width; x++)
				{
					uint32_t index = y * bitmap.width + x;
					rgb_t pal = bitmap.palette[bitmap.pixel_data[index]];
					
					gfx.data[(bitmap.height - (y+1)) * bitmap.width + x] = ((pal.r >> 3) << 11) | ((pal.g >> 2) << 5) | (pal.b >> 3);
				}
			break;
		case 16:
			for (int y = 0; y < bitmap.height; y++)
				for (int x = 0; x < bitmap.width*2; x+=2)
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
			for (int y = 0; y < bitmap.height; y++)
				for (int x = 0; x < bitmap.width*3; x+=3)
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
	
	return gfx;
}
