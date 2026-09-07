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
// UPDATE: Only used for LEGACY GFX now!!
// UPDATE 2: unless you wanna use it for misc stuff
void V_LoadPalette(const char *filename, uint16_t* pal)
{
	int i;
	long size;
	FILE *file = fopen(filename, "rb");
	
	if (file == NULL)
		I_Error("Failed to read palette file!\n");
	
	fseek(file, 0L, SEEK_END);
	size = ftell(file);
	rewind(file);
	
	pal = malloc((size/3 + 1) * sizeof(uint16_t));
	
	pal[0] = 0; // pitch black!
	
	for (i = 0; i < size/3; i++)
	{
		uint8_t r, g, b;
		
		fread(&r, 1, 1, file);
		fread(&g, 1, 1, file);
		fread(&b, 1, 1, file);
		
		pal[i+1] = ((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3);
	}
	
	fclose(file);
}

void V_Free(void)
{
	if (palette != NULL)
		free(palette);

	free(vid.buffer);
}

void V_ClearScreen(void)
{
	uint16_t i;
	for (i = 0; i < VID_WIDTH*VID_HEIGHT; i++)
		vid.buffer[i] = 0;
}

void V_DrawDot(int16_t x, int16_t y, uint16_t col)
{	
	if (x < 0 || y < 0 || x >= VID_WIDTH || y >= VID_HEIGHT || col == 0)
        return;
	
	vid.buffer[x+(y*VID_WIDTH)] = col;
}

void V_DrawLine(int16_t x, int16_t y, int32_t angle, uint16_t length, uint16_t col)
{	
	uint16_t i;

	if (col == 0)
		return; // bow wow wow yippee yo yippee yay

	for (i = 0; i < length; i++)
	{
		int16_t px = x, py = y;

		px += i * cos((angle-90) * (PI/180.0));
		py += i * sin((angle-90) * (PI/180.0));

		if (px < 0 || py < 0 || px >= VID_WIDTH || py >= VID_HEIGHT)
       		return;

		vid.buffer[px+(py*VID_WIDTH)] = col;
	}
}

void V_DrawBox(int16_t x, int16_t y, int32_t angle, uint16_t width, uint16_t height, uint16_t col)
{
	int16_t offx, offy, offx2, offy2;
	double rad_width = (angle + 90) * (PI/180.0);
	double rad_height = (angle + 180) * (PI/180.0);

	V_DrawLine(x, y, angle+180, height, col);
	V_DrawLine(x, y, angle+90, width, col);

	offx = (int16_t)(-sin(rad_height) * height);
    offy = (int16_t)(-cos(rad_height) * height); 
    offx2 = (int16_t)(-sin(rad_width) * width);
    offy2 = (int16_t)(-cos(rad_width) * width);

	V_DrawLine(x - offx2 - 1, y + offy2, angle+180, height, col);
	V_DrawLine(x - offx, y + offy - 1, angle+90, width, col);
}

void V_DrawCropped2x(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t flags)
{	
	int zx, zy;

	if ((gfx.width * gfx.height) <= 0)
        return;
	
	// completely out of bounds
	if (x*2 >= VID_WIDTH || y*2 >= VID_HEIGHT || x*2+w*2 < 0 || y*2+h*2 < 0)
		return;
	
	for (zy = 0; zy < h; zy++)
		for (zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			int vx = x*2 + zx*2;
			int vy = y*2 + zy*2;
			
			if (flags & V_SMALL)
			{
				vx -= zx;
				vy -= zy;
			}
			
			if (i >= gfx.width * gfx.height)
				return;
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*VID_WIDTH)] = gfx.data[i];
			vid.buffer[vx+1+(vy*VID_WIDTH)] = gfx.data[i];
			vid.buffer[vx+((vy+1)*VID_WIDTH)] = gfx.data[i];
			vid.buffer[vx+1+((vy+1)*VID_WIDTH)] = gfx.data[i];
		}
}

void V_DrawCroppedNoCheck(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t flags)
{	
	int zx, zy;

	for (zy = 0; zy < h; zy++)
		for (zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			int vx = x + zx;
			int vy = y + zy;
			
			if (flags & V_SMALL)
			{
				vx -= zx/2;
				vy -= zy/2;
			}

			if (i >= gfx.width * gfx.height)
				return;
			
			if (gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*VID_WIDTH)] = gfx.data[i];
		}
}

void V_DrawCropped(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t flags)
{	
	int zx, zy;

	if ((gfx.width * gfx.height) <= 0)
        return;
	
	// completely out of bounds
	if (x >= VID_WIDTH || y >= VID_HEIGHT || x+w < 0 || y+h < 0)
		return;
	
	// completely within bounds
	if (x >= 0 && y >= 0 && x+w < VID_WIDTH && y+h < VID_HEIGHT) {
		V_DrawCroppedNoCheck(gfx, x, y, sx, sy, w, h, flags);
		return;
	}
	
	for (zy = 0; zy < h; zy++)
		for (zx = 0; zx < w; zx++)
		{
			int i = sx + sy*gfx.width + zx + zy*gfx.width;
			int vx = x + zx;
			int vy = y + zy;
			
			if (flags & V_SMALL)
			{
				vx -= zx/2;
				vy -= zy/2;
			}
			
			if (i >= gfx.width * gfx.height)
				return;
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*VID_WIDTH)] = gfx.data[i];
		}
}

void V_Draw(gfx_t gfx, int16_t x, int16_t y, uint32_t flags)
{
	V_DrawCropped(gfx, x, y, 0, 0, gfx.width, gfx.height, flags);
}

// Text functions

void V_DrawTextFromFont(font_t font, const char* string, int16_t x, int16_t y, uint32_t flags)
{
	int16_t bx = x, by = y;
	uint8_t charw = font.charsize >> 8, charh = font.charsize & 0xFF;
	int i;

	for (i = 0; i < strlen(string); i++) {
		int c = (int)string[i];
		int8_t xoff = font.offset[c] >> 8, yoff = font.offset[c] & 0xFF;
		uint8_t w = font.size[c] >> 8, h = font.size[c] & 0xFF;
		
		if (y + yoff > VID_HEIGHT)
			return;
		
		if ((string[i] == '\n') || (x+w >= VID_WIDTH && c != 32)) {
			x = bx;
			y += charh+1;
		}
		
		if (y + yoff + charh < 0) {
			x += w;
			continue;
		}
		
		if (c >= 32) {
			c -= 33;
			
			if (flags & V_WAVYTEXT)
				yoff += abs((game_tick + x) % FRAMERATE - FRAMERATE/2)/2 - charh/2;
			
			if (flags & V_WAVYTEXTTWO)
				yoff += abs((game_tick + i) % FRAMERATE - FRAMERATE/2)/2 - charh/2;
			
			if (flags & V_JUMPYTEXT)
				yoff += abs((game_tick/2 + x*w) % h - h/2)/2 - charh/2 + 2;
				
			if (c > -1)
				V_DrawCropped(font.gfx, x + xoff, y + yoff, (c % 16) * charw, (c / 16) * charh, charw, charh, flags);
			
			x += w;
		}
	}
}

void V_DrawText(const char* string, int16_t x, int16_t y, uint32_t flags)
{
	V_DrawTextFromFont(font_default, string, x, y, flags);
}
