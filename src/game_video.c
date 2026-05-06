// JADEFRACTURE
// game_video.c

#include "i_system.h"

#include "game_gfx.h"
#include "game_video.h"

vid_t vid;
uint16_t* palette;

void V_Init(void)
{
	vid.width = BASEVIDWIDTH;
	vid.height = BASEVIDHEIGHT;
	
	vid.buffer = malloc(BASEVIDWIDTH*BASEVIDHEIGHT);
	memset(vid.buffer, 0, BASEVIDWIDTH*BASEVIDHEIGHT);
}

// Load the palette into vid.palette :3 Nozomi
void V_LoadPalette(void)
{
	long size;
	FILE *file = fopen("data/palette.mpl", "rb");
	
	if (file == NULL)
	{
		I_Error("Failed to read palette file!\n");
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
		
		#if defined(NDS)
		palette[i+1] = rgb565_to_rgb15(palette[i+1]);
		#endif
	}
	
	fclose(file);
}

void V_ClearScreen(void)
{
	for (int i = 0; i < BASEVIDWIDTH*BASEVIDHEIGHT; i++)
		vid.buffer[i] = 0;
}

void V_DrawDot(int x, int y, uint8_t col)
{
	if (x < 0 || y < 0 || x >= BASEVIDWIDTH || y >= BASEVIDHEIGHT)
        return;
	
	if (col > 38)
		col = 38;
	
	vid.buffer[x+(y*BASEVIDWIDTH)] = col;
}

void V_Draw(gfx_t gfx, int x, int y)
{
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int i = 0; i < gfx.size; i++)
	{
		int vx = x + (i % gfx.width) - gfx.xoff;
		int vy = y + (i / gfx.width) - gfx.yoff;
		
		if (vx < 0 || vy < 0 || vx >= BASEVIDWIDTH || vy >= BASEVIDHEIGHT || ALPHA_INDEX(gfx.data[i]))
			continue;
		
		vid.buffer[vx+(vy*BASEVIDWIDTH)] = PAL_INDEX(gfx.data[i]);
	}
}

void V_DrawCropped(gfx_t gfx, int x, int y, int sx, int sy, int w, int h)
{	
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int zy = 0; zy < h; zy++)
		for (int zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			int vx = x + zx - gfx.xoff;
			int vy = y + zy - gfx.yoff;
			
			if (vx < 0 || vy < 0 || vx >= BASEVIDWIDTH || vy >= BASEVIDHEIGHT || ALPHA_INDEX(gfx.data[i]))
				continue;
			
			vid.buffer[vx+(vy*BASEVIDWIDTH)] = PAL_INDEX(gfx.data[i]);
		}
}
