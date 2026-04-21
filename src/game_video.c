// JADEFRACTURE
// game_video.c

#include "game_gfx.h"
#include "game_video.h"

vid_t vid;
uint16_t* palette;

void V_Init(void)
{
	vid.width = BASEVIDWIDTH;
	vid.height = BASEVIDHEIGHT;
	
	vid.buffer = malloc(vid.width*vid.height);
	memset(vid.buffer, 0, vid.width*vid.height);
}

// Load the palette into vid.palette :3 Nozomi
void V_LoadPalette(void)
{
	long size;
	FILE *file = fopen("data/palette.mpl", "rb");
	
	if (file == NULL)
	{
		printf("Failed to read palette file!\n");
		exit(-1);
	}
	
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	rewind(file);
	
	palette = malloc((size/3 + 1) * sizeof(uint16_t));
	
	palette[0] = 0; // pitch black!
	
	for (int i = 0; i < size/3; i++)
	{
		uint8_t r, g, b;
		
		fread(&r, 1, 1, file);
		fread(&g, 1, 1, file);
		fread(&b, 1, 1, file);
		
		palette[i+1] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	}
	
	fclose(file);
}

void V_DrawDot(int x, int y, uint8_t col)
{
	if (x < 0 || y < 0 || x >= vid.width || y >= vid.height)
        return;
	
	if (col > 37)
		col = 38;
	
	vid.buffer[x+(y*vid.width)] = col;
}

void V_DrawSprite(gfx_t gfx, int x, int y)
{
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int i = 0; i < gfx.size; i++)
	{
		if (ALPHA_INDEX(gfx.data[i]))
			continue;
		
		V_DrawDot(x + (i % gfx.width) - gfx.xoff, y + (i / gfx.width) - gfx.yoff, PAL_INDEX(gfx.data[i]));
	}
}

void V_DrawCroppedSprite(gfx_t gfx, int x, int y, int sx, int sy, int w, int h)
{
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int zy = 0; zy < h; zy++)
		for (int zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			
			if (ALPHA_INDEX(gfx.data[i]))
				continue;
			
			V_DrawDot(x + zx - gfx.xoff, y + zy - gfx.yoff, PAL_INDEX(gfx.data[i]));
		}
}