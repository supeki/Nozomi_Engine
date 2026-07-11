// Nozomi Engine
// game_font.c

#include "game_defs.h"
#include "game_gfx.h"
#include "game_font.h"
#include "game_input.h"
#include "game_video.h"

#include "i_system.h"

font_t FNT_LoadFont(const char *filename)
{
	FILE *fp = fopen(filename, "rb");
	font_t font;
	char gfx_name[33];
	
	fread(gfx_name, sizeof(char), 32, fp);
	fread(&font.bitmap, sizeof(bool), 1, fp);

	if (font.bitmap)
		font.bmpgfx = BMPGFX_LoadBitmap(va("data/fonts/%s.bmp", gfx_name));
	else
		font.gfx = GFX_LoadGFX(va("data/fonts/%s.gfx", gfx_name));

	fread(&font.charsize, sizeof(uint16_t), 1, fp);
	font.offset = malloc(256*sizeof(int16_t));
	font.size = malloc(256*sizeof(uint16_t));
	
	for (int i = 0; i < 256; i++)
		fread(&font.offset[i], sizeof(int16_t), 1, fp);
	for (int i = 0; i < 256; i++)
		fread(&font.size[i], sizeof(uint16_t), 1, fp);

	return font;
}

font_t temp_font;
bool font_edit = false;
uint8_t curchar = 0;

void FNT_StartFontEdit(void)
{
	temp_font = FNT_LoadFont("data/fonts/default.fnt");
	font_edit = true;
}

void FNT_FontEditUpdate(void)
{
	uint8_t lr = G_ControlDown(CON_RIGHT, true) - G_ControlDown(CON_LEFT, true);
	uint8_t ud = G_ControlDown(CON_DOWN, true) - G_ControlDown(CON_UP, true);
	
	uint8_t  xoff = temp_font.offset[curchar] >> 8, yoff = temp_font.offset[curchar] & 0xFF;
	uint8_t w = temp_font.size[curchar] >> 8, h = temp_font.size[curchar] & 0xFF;
	uint8_t charw = temp_font.charsize >> 8, charh = temp_font.charsize & 0xFF;
	
	uint8_t  txoff = xoff, tyoff = yoff;
	uint8_t tw = w, th = h;
	uint8_t tcharw = charw, tcharh = charh;
	uint8_t oldchar = curchar;

	if (lr != 0)
	{
		if (G_ControlDown(CON_A, false)) {
			txoff += lr;
		} else if (G_ControlDown(CON_B, false)) {
			tw += lr;
		} else if (G_ControlDown(CON_C, false)) {
			tcharw += lr;
		} else {
			curchar += lr;
		}
	}
	
	if (ud != 0)
	{
		if (G_ControlDown(CON_A, false)) {
			tyoff += ud;
		} else if (G_ControlDown(CON_B, false)) {
			th += ud;
		} else if (G_ControlDown(CON_C, false)) {
			tcharh += ud;
		} else {
			curchar += ud*16;
		}
	}
	
	if (G_ControlDown(CON_START, true))
		FNT_SaveTempFont();
	
	if (curchar < 0)
		curchar = 0;
	else if (curchar > 255)
		curchar = 255;
	
	uint16_t tsize = (tw << 8) + th;
	uint16_t tcharoffset = (txoff << 8) + tyoff;
	uint16_t tcharsize = (tcharw << 8) + tcharh;
	
	if (curchar == oldchar) {
		if (tsize != temp_font.size[curchar])
			temp_font.size[curchar] = tsize;
		
		if (tcharoffset != temp_font.offset[curchar])
			temp_font.offset[curchar] = tcharoffset;
		
		if (tcharsize != temp_font.charsize)
			temp_font.charsize = tcharsize;
	}
}

static void FNT_DrawCroppedDouble(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h)
{	
	if ((gfx.size + gfx.width) <= 0)
        return;
	
	for (int zy = 0; zy < h; zy++)
		for (int zx = 0; zx < w; zx++)
		{
			uint32_t i = sx + sy*gfx.width + zx + zy*gfx.width;
			int32_t vx = x + zx*2 - gfx.xoff*2;
			int32_t vy = y + zy*2 - gfx.yoff*2;
						
			if (i >= gfx.size)
				return;
			
			if (vx < 0 || vy < 0 || vx >= vid.width || vy >= vid.height || gfx.data[i] == 0)
				continue;
			
			vid.buffer[vx+(vy*vid.width)] = palette[gfx.data[i]];
			vid.buffer[vx+(vy*vid.width)+1] = palette[gfx.data[i]];
			vid.buffer[vx+(vy*vid.width)+vid.width] = palette[gfx.data[i]];
			vid.buffer[vx+(vy*vid.width)+vid.width+1] = palette[gfx.data[i]];
		}
}

void FNT_FontEditDraw(void)
{
	uint8_t charw = temp_font.charsize >> 8, charh = temp_font.charsize & 0xFF;
	
	uint8_t col = abs((I_GetTicks()/10) % 4 - 2) + 8; 
	
	for (int i = 0; i < 256; i++)
	{
		V_DrawTextFromFont(temp_font, 
			va("%c", i),
			(i % 16) * charw, (i / 16) * charh, 0);
	}
	
	for (int y = 0; y < charh-1; y++)
		V_DrawDot((curchar % 16) * charw + charw/2-1, (curchar / 16) * charh + y, col);
	for (int x = 0; x < charw-1; x++)
		V_DrawDot((curchar % 16) * charw + x, (curchar / 16) * charh + charh/2-1, col);
	
	int8_t  xoff = temp_font.offset[curchar] >> 8, yoff = temp_font.offset[curchar] & 0xFF;
	uint8_t w = temp_font.size[curchar] >> 8, h = temp_font.size[curchar] & 0xFF;
	
	for (int x = 0; x < charw*2 + 2; x++) {
		V_DrawDot(vid.width - charw*4 - 1 + x, 2*charh - 1, col);
		V_DrawDot(vid.width - charw*4 - 1 + x, 2*charh + 2*charh, col);
	}
	
	for (int y = 0; y < charh*2 + 2; y++) {
		V_DrawDot(vid.width - charw*4 - 1, 2*charh-1 + y, col);
		V_DrawDot(vid.width - charw*4 + charw*2, 2*charh-1 + y, col);
	}
	
	if (curchar >= 33) {
		int cc = curchar - 33;
		FNT_DrawCroppedDouble(font_default.gfx, vid.width - charw*4 + xoff*2, 2*charh + yoff*2, (cc % 10) * charw, (cc / 10) * charh, charw, charh);
	}

	V_DrawText(va("Offsets: %d, %d", xoff, yoff), 0, charw*charh+charh, 0);
	V_DrawText(va("Char Size (wxh): %d, %d", w, h), 0, charw*charh+charh*2, 0);
	V_DrawText(va("Cell Size (wxh): %d, %d", charw, charh), 0, charw*charh+charh*3, 0);
	
	V_DrawTextFromFont(temp_font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nZA 01234567890", 0, vid.height - charh*6, 0);
	V_DrawTextFromFont(temp_font, "abcdefghijklmnopqrstuvwxyz\nza .,/\\;:\'\"[]{}-_=+!@#$%^&*()~|", 0, vid.height - charh*4, 0);
	V_DrawTextFromFont(temp_font, "The quick brown fox\njumped over the lazy dog.", 0, vid.height - charh*2, 0);
}

void FNT_SaveTempFont(void)
{
	FILE *fp = fopen("data/fonts/default.fnt", "wb+");
	char gfx_name[33];
	bool bitmap = false;

	snprintf(gfx_name, 32, "default");
	fwrite(gfx_name, sizeof(gfx_name)-1, 1, fp);
	fwrite(&temp_font.bitmap, sizeof(bool), 1, fp);
	fwrite(&temp_font.charsize, sizeof(uint16_t), 1, fp);
	
	for (int i = 0; i < 256; i++)
		fwrite(&temp_font.offset[i], sizeof(int16_t), 1, fp);
	for (int i = 0; i < 256; i++)
		fwrite(&temp_font.size[i], sizeof(uint16_t), 1, fp);

	fclose(fp);
}