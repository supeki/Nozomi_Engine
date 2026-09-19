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

	font_default = FNT_LoadFont(va("%s/data/fonts/default.fnt", I_GetHomeDir()));
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

uint16_t V_MixColors(uint16_t c, uint16_t c2, uint8_t a) 
{
    uint8_t r, g, b, r2, g2, b2, r_out, g_out, b_out;

    r = (c >> 11) & 0x1F;
    g = (c >> 5) & 0x3F;
    b = c & 0x1F;

    r2 = (c2 >> 11) & 0x1F;
    g2 = (c2 >> 5) & 0x3F;
    b2 = c2 & 0x1F;

    r_out = (r*(255-a) + r2*a) >> 8;
    g_out = (g*(255-a) + g2*a) >> 8;
    b_out = (b*(255-a) + b2*a) >> 8;

    return (r_out << 11) | (g_out << 5) | b_out;
}

void V_Free(void)
{
	if (palette != NULL)
		free(palette);

	free(vid.buffer);
}

void V_FillScreen(uint16_t col)
{
	uint32_t i;
	for (i = 0; i < VID_WIDTH*VID_HEIGHT; i++)
		vid.buffer[i] = col;
}

void V_DrawDot(int32_t x, int32_t y, uint16_t col, uint32_t flags)
{	
	uint16_t *dest;

	if (x < 0 || y < 0 || x >= VID_WIDTH || y >= VID_HEIGHT || col == 0)
        return;

	dest = &vid.buffer[x + (y*VID_WIDTH)];
	
	if (flags & V_HALFTRANS)
		*dest = V_MixColors(*dest, col, 127);
	else
		*dest = col;
}

void V_DrawLine(int32_t x, int32_t y, int32_t angle, uint16_t length, uint16_t col, uint32_t flags)
{	
	uint16_t i;
	int32_t px = x << 16, py = y << 16;
	int32_t dx, dy;
	uint16_t *dest;

	if (col == 0)
		return; // bow wow wow yippee yo yippee yay

	dx = (int32_t)(cos((angle-90) * (PI/180.0)) * 65536);
	dy = (int32_t)(sin((angle-90) * (PI/180.0)) * 65536);

	for (i = 0; i < length; i++)
	{ 
		int16_t vx = px >> 16, vy = py >> 16;

		if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT)
       		return;

		dest = &vid.buffer[vx + (vy*VID_WIDTH)];

		if (flags & V_HALFTRANS)
			*dest = V_MixColors(*dest, col, 127);
		else
			*dest = col;

		px += dx;
		py += dy;
	}
}

void V_DrawBox(int32_t x, int32_t y, int32_t angle, uint16_t width, uint16_t height, uint16_t col, uint32_t flags)
{
	int32_t offx, offy, offx2, offy2;
	double rad_width = (angle + 90) * (PI/180.0);
	double rad_height = (angle + 180) * (PI/180.0);

	V_DrawLine(x, y, angle+180, height, col, flags);
	V_DrawLine(x, y, angle+90, width, col, flags);

	offx = (int32_t)(-sin(rad_height) * height);
    offy = (int32_t)(-cos(rad_height) * height); 
    offx2 = (int32_t)(-sin(rad_width) * width);
    offy2 = (int32_t)(-cos(rad_width) * width);

	V_DrawLine(x - offx2 - 1, y + offy2, angle+180, height, col, flags);
	V_DrawLine(x - offx, y + offy - 1, angle+90, width, col, flags);
}

void V_DrawCroppedAnimated(gfx_t gfx, int32_t x, int32_t y, int32_t sx, int32_t sy, uint16_t w, uint16_t h, uint32_t frames, uint32_t fps, uint32_t flags)
{	
	uint32_t ticks = FRAMERATE / fps;
	sx += ((I_GetTicks()/ticks) % frames)*w;
	V_DrawCropped(gfx, x, y, sx, sy, w, h, flags);
}

void V_DrawCroppedScaled(gfx_t gfx, int32_t x, int32_t y, int32_t sx, int32_t sy, uint16_t w, uint16_t h, uint32_t scale, uint32_t flags)
{	
	int vx, vy, zx, zy, dx, dy, px, py;
	int32_t dest_x, dest_y = 0;
	uint16_t *src, *dest;

	if ((gfx.width * gfx.height) <= 0)
        return;
	
	// completely out of bounds
	if (x >= VID_WIDTH || y >= VID_HEIGHT || x+(w*scale >> 16) < 0 || y+(h*scale >> 16) < 0 || scale == 0)
		return;
	
	for (zy = 0; zy < h; zy++) {
		dest_x = 0;

		for (zx = 0; zx < w; zx++) {

			src = &gfx.data[sx + sy*gfx.width + zx + zy*gfx.width];

			vx = x + (dest_x >> 16);
			vy = y + (dest_y >> 16);
			
			dx = x + ((dest_x+scale) >> 16);
			dy = y + ((dest_y+scale) >> 16);
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || vy >= VID_HEIGHT || *src == 0) {
				dest_x += scale;
				continue;
			}

			for (py = vy; py < dy; py++)
			{
				if (py < 0 || py >= VID_HEIGHT)
					continue;

				dest = &vid.buffer[vx+(py*VID_WIDTH)];

				for (px = vx; px < dx; px++)
				{
					if (px < 0 || px >= VID_WIDTH)
						continue;

					if (flags & V_HALFTRANS)
						*dest = V_MixColors(*dest, *src, 127);
					else
						*dest = *src;

					dest++;
				}
			}

			dest_x += scale;
		}

		dest_y += scale;
	}
}

void V_DrawCropped(gfx_t gfx, int32_t x, int32_t y, int32_t sx, int32_t sy, uint16_t w, uint16_t h, uint32_t flags)
{	
	int zx, zy, vx, vy;
	uint16_t *src, *dest;

	if ((gfx.width * gfx.height) <= 0)
        return;
	
	// completely out of bounds
	if (x >= VID_WIDTH || y >= VID_HEIGHT || x+w < 0 || y+h < 0)
		return;
	
	for (zy = 0; zy < h; zy++)
		for (zx = 0; zx < w; zx++)
		{
			src = &gfx.data[sx + sy*gfx.width + zx + zy*gfx.width];
			
			vx = x + zx;
			vy = y + zy;
			
			if (vy > VID_HEIGHT)
				return;
			
			if (vx < 0 || vy < 0 || vx >= VID_WIDTH || *src == 0)
				continue;

			dest = &vid.buffer[vx+(vy*VID_WIDTH)];
			
			if (flags & V_HALFTRANS)
				*dest = V_MixColors(*dest, *src, 127);
			else
				*dest = *src;
		}
}

void V_DrawTiled(gfx_t gfx, int32_t x, int32_t y, uint32_t flags)
{
	int32_t sx, sy;

	for (sy = y; sy < VID_HEIGHT; sy += gfx.height)
		for (sx = x; sx < VID_WIDTH; sx += gfx.width)
			V_Draw(gfx, sx, sy, flags);
}

void V_Draw(gfx_t gfx, int32_t x, int32_t y, uint32_t flags)
{
	V_DrawCropped(gfx, x, y, 0, 0, gfx.width, gfx.height, flags);
}

// Text functions

void V_DrawTextFromFont(font_t font, const char* string, int32_t x, int32_t y, uint32_t flags)
{
	int32_t bx = x, by = y;
	uint8_t charw = font.charsize >> 8, charh = font.charsize & 0xFF;
	int i;

	for (i = 0; string[i] != '\0'; i++) {
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

void V_DrawText(const char* string, int32_t x, int32_t y, uint32_t flags)
{
	V_DrawTextFromFont(font_default, string, x, y, flags);
}
