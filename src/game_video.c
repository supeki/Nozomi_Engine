// Nozomi Engine
// game_video.c

#include "i_system.h"

#include "game_font.h"
#include "game_gfx.h"
#include "game_main.h"
#include "game_video.h"

vid_t vid;
uint16_t* palette;

font_t font_default;

void V_Init(void)
{
	vid.buffer = malloc(VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));
	memset(vid.buffer, 0, VID_WIDTH * VID_HEIGHT * sizeof(uint16_t));

	font_default = FNT_LoadFont("data/fonts/default.fnt");
}

// Load the palette into vid.palette :3 Nozomi
void V_LoadPalette(void)
{
	long size;
	FILE *file = fopen("data/palette.mpl", "rb");
	
	if (file == NULL)
		I_Error("Failed to read palette file!\n");
	
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

void V_ClearScreen(void)
{
	for (uint16_t i = 0; i < VID_WIDTH*VID_HEIGHT; i++)
		vid.buffer[i] = palette[0];
}

void V_DrawDot(int16_t x, int16_t y, uint8_t col)
{	
	if (x < 0 || y < 0 || x >= VID_WIDTH || y >= VID_HEIGHT || col == 0)
        return;
	
	if (col > 38)
		col = 38;
	
	vid.buffer[x+(y*VID_WIDTH)] = palette[col];
}

void V_Draw(gfx_t gfx, int16_t x, int16_t y)
{
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int i = 0; i < gfx.size; i++)
	{
		int32_t vx = x + (i % gfx.width) - gfx.xoff;
		int32_t vy = y + (i / gfx.width) - gfx.yoff;
		
		if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
			continue;
		
		vid.buffer[vx+(vy*VID_WIDTH)] = palette[gfx.data[i]];
	}
}

void V_DrawCropped(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h)
{	
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int zy = 0; zy < h; zy++)
		for (int zx = 0; zx < w; zx++)
		{
			uint32_t i = sx + sy*gfx.width + zx + zy*gfx.width;
			int32_t vx = x + zx - gfx.xoff;
			int32_t vy = y + zy - gfx.yoff;
						
			if (i >= gfx.size)
				return;
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*VID_WIDTH)] = palette[gfx.data[i]];
		}
}

// Bitmap variants if need be

void V_DrawBitmap(bitmap_gfx_t gfx, int16_t x, int16_t y)
{
	if ((gfx.width * gfx.height) <= 0)
		return;
	
	for (int i = 0; i < (gfx.width * gfx.height); i++)
	{
		int vx = x + (i % gfx.width);
		int vy = y + (i / gfx.width);
		
		if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
			continue;
		
		vid.buffer[vx+(vy*VID_WIDTH)] = gfx.data[i];
	}
}

void V_DrawCroppedBitmap(bitmap_gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h)
{	
	if ((gfx.width * gfx.height) <= 0)
        return;
	
	for (int zy = 0; zy < h; zy++)
		for (int zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			int vx = x + zx;
			int vy = y + zy;
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*VID_WIDTH)] = gfx.data[i];
		}
}

// Text functions


void V_DrawTextFromFont(font_t font, const char* string, int16_t x, int16_t y, int flags)
{
	int16_t bx = x, by = y;
	uint8_t charw = font.charsize >> 8, charh = font.charsize & 0xFF;
	
	
	for (int i = 0; i < strlen(string); i++) {
		int c = (int)string[i];
		
		if (c >= 32) {
			int8_t xoff = font.offset[c] >> 8, yoff = font.offset[c] & 0xFF;
			uint8_t w = font.size[c] >> 8, h = font.size[c] & 0xFF;
			
			c -= 33;
			
			if (flags & V_WAVYTEXT)
			{
				yoff += abs((game_tick + x) % FRAMERATE - FRAMERATE/2)/2 - charw;
			}
				
			if (c > -1) {
				if (font.bitmap)
					V_DrawCroppedBitmap(font.bmpgfx, x + xoff, y + yoff, (c % 16) * charw, (c / 16) * charh, charw, charh);
				else
					V_DrawCropped(font.gfx, x + xoff, y + yoff, (c % 10) * charw, (c / 10) * charh, charw, charh);
			}
			
			x += w;
		}
		
		if ((string[i] == '\n') || (x >= VID_WIDTH)) {
			x = bx;
			y += charh+1;
		}
	}
}

void V_DrawText(const char* string, int16_t x, int16_t y, int flags)
{
	V_DrawTextFromFont(font_default, string, x, y, flags);
}