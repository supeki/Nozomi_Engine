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
	font_t font;
	FILE *fp = fopen(filename, "rb");
	char gfx_name[33];
	int i;
	
	if (fp == NULL)
		I_Error("Failed to read font file: %s\n", filename);

	fread(gfx_name, sizeof(char), 32, fp);
	gfx_name[32] = '\0';
	font.gfx = GFX_LoadGFX(va("%s/data/fonts/%s.bmp", I_GetHomeDir(), gfx_name));

	fread(&font.charsize, sizeof(uint16_t), 1, fp);
	font.offset = malloc(256*sizeof(int16_t));
	font.size = malloc(256*sizeof(uint16_t));
	
	for (i = 0; i < 256; i++)
		fread(&font.offset[i], sizeof(int16_t), 1, fp);
	for (i = 0; i < 256; i++)
		fread(&font.size[i], sizeof(uint16_t), 1, fp);

	return font;
}

font_t temp_font;
bool font_edit = false;
uint8_t curchar = 0;

void FNT_StartFontEdit(void)
{
	temp_font = FNT_LoadFont(va("%s/data/fonts/default.fnt", I_GetHomeDir()));
	font_edit = true;
}

void FNT_FontEditUpdate(void)
{
	int8_t lr = G_ControlDown(PLAYER_ONE, CON_RIGHT, true) - G_ControlDown(PLAYER_ONE, CON_LEFT, true);
	int8_t ud = G_ControlDown(PLAYER_ONE, CON_DOWN, true) - G_ControlDown(PLAYER_ONE, CON_UP, true);
	
	uint8_t  xoff = temp_font.offset[curchar] >> 8, yoff = temp_font.offset[curchar] & 0xFF;
	uint8_t w = temp_font.size[curchar] >> 8, h = temp_font.size[curchar] & 0xFF;
	uint8_t charw = temp_font.charsize >> 8, charh = temp_font.charsize & 0xFF;
	
	uint8_t  txoff = xoff, tyoff = yoff;
	uint8_t tw = w, th = h;
	uint8_t tcharw = charw, tcharh = charh;
	uint8_t oldchar = curchar;

	uint16_t tsize, tcharoffset, tcharsize;

	if (lr != 0)
	{
		if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
			txoff += lr;
		} else if (G_ControlDown(PLAYER_ONE, CON_B, false)) {
			tw += lr;
		} else if (G_ControlDown(PLAYER_ONE, CON_C, false)) {
			tcharw += lr;
		} else {
			curchar += lr;
		}
	}
	
	if (ud != 0)
	{
		if (G_ControlDown(PLAYER_ONE, CON_A, false)) {
			tyoff += ud;
		} else if (G_ControlDown(PLAYER_ONE, CON_B, false)) {
			th += ud;
		} else if (G_ControlDown(PLAYER_ONE, CON_C, false)) {
			tcharh += ud;
		} else {
			curchar += ud*16;
		}
	}
	
	if (G_ControlDown(PLAYER_ONE, CON_START, true))
		FNT_SaveTempFont();
	
	if (curchar < 0)
		curchar = 0;
	else if (curchar > 255)
		curchar = 255;
	
	tsize = (tw << 8) + th;
	tcharoffset = (txoff << 8) + tyoff;
	tcharsize = (tcharw << 8) + tcharh;

	if (curchar == oldchar) {
		if (tsize != temp_font.size[curchar])
			temp_font.size[curchar] = tsize;
		
		if (tcharoffset != temp_font.offset[curchar])
			temp_font.offset[curchar] = tcharoffset;
		
		if (tcharsize != temp_font.charsize)
			temp_font.charsize = tcharsize;
	}
}

void FNT_FontEditDraw(void)
{
	uint8_t charw = temp_font.charsize >> 8, charh = temp_font.charsize & 0xFF;
	int8_t  xoff, yoff;
	uint8_t w, h;
	uint8_t col = abs((I_GetTicks()/10) % 4 - 2) + 8; 
	int i, y, x;

	for (i = 0; i < 256; i++)
	{
		V_DrawTextFromFont(temp_font, 
			va("%c", i),
			(i % 16) * charw, (i / 16) * charh, 0);
	}
	
	for (y = 0; y < charh-1; y++)
		V_DrawDot((curchar % 16) * charw + charw/2-1, (curchar / 16) * charh + y, col);
	for (x = 0; x < charw-1; x++)
		V_DrawDot((curchar % 16) * charw + x, (curchar / 16) * charh + charh/2-1, col);
	
	xoff = temp_font.offset[curchar] >> 8;
	yoff = temp_font.offset[curchar] & 0xFF;
	w = temp_font.size[curchar] >> 8;
	h = temp_font.size[curchar] & 0xFF;
	
	for (x = 0; x < charw*2 + 2; x++) {
		V_DrawDot(VID_WIDTH - charw*4 - 1 + x, 2*charh - 1, col);
		V_DrawDot(VID_WIDTH - charw*4 - 1 + x, 2*charh + 2*charh, col);
	}
	
	for (y = 0; y < charh*2 + 2; y++) {
		V_DrawDot(VID_WIDTH - charw*4 - 1, 2*charh-1 + y, col);
		V_DrawDot(VID_WIDTH - charw*4 + charw*2, 2*charh-1 + y, col);
	}

	V_DrawText(va("Offsets: %d, %d", xoff, yoff), 0, charw*charh+charh, 0);
	V_DrawText(va("Char Size (wxh): %d, %d", w, h), 0, charw*charh+charh*2, 0);
	V_DrawText(va("Cell Size (wxh): %d, %d", charw, charh), 0, charw*charh+charh*3, 0);
	
	V_DrawTextFromFont(temp_font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nZA 01234567890", 0, VID_HEIGHT - charh*6, 0);
	V_DrawTextFromFont(temp_font, "abcdefghijklmnopqrstuvwxyz\nza .,/\\;:\'\"[]{}-_=+!@#$%^&*()~|", 0, VID_HEIGHT - charh*4, 0);
	V_DrawTextFromFont(temp_font, "The quick brown fox\njumped over the lazy dog.", 0, VID_HEIGHT - charh*2, 0);
}

void FNT_SaveTempFont(void)
{
	FILE *fp = fopen(va("%s/data/fonts/temp.fnt", I_GetHomeDir()), "wb+");
	char gfx_name[33];
	int i;

	snprintf(gfx_name, 32, "temp");
	fwrite(gfx_name, sizeof(gfx_name)-1, 1, fp);
	fwrite(&temp_font.charsize, sizeof(uint16_t), 1, fp);
	
	for (i = 0; i < 256; i++)
		fwrite(&temp_font.offset[i], sizeof(int16_t), 1, fp);
	for (i = 0; i < 256; i++)
		fwrite(&temp_font.size[i], sizeof(uint16_t), 1, fp);

	fclose(fp);
}
