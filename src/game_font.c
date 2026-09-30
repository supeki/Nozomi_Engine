// Nozomi Engine
// game_font.c

#include "game_defs.h"
#include "game_gfx.h"
#include "game_font.h"
#include "game_input.h"
#include "game_video.h"

#include "i_system.h"

static char temp_gfx_name[33];

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

	font.gfx = GFX_LoadGFX(va("%sdata/fonts/%s.bmp", I_GetHomeDir(), gfx_name));
	sprintf(temp_gfx_name, gfx_name);
	font.charsize = FIL_ReadU16(fp);
	font.offset = malloc(256*sizeof(uint16_t));
	font.size = malloc(256*sizeof(uint16_t));
	
	for (i = 0; i < 256; i++)
		font.offset[i] = FIL_ReadU16(fp);
	for (i = 0; i < 256; i++)
		font.size[i] = FIL_ReadU16(fp);

	fclose(fp);
	return font;
}

font_t temp_font;
bool font_edit = false;
static uint8_t curchar = 0;
static bool file_menu = false;
static dirfiles_t font_dirfiles;
static char temp_font_name[33];

static font_t FNT_CreateFontFromGfx(const char *filename, uint8_t cw, uint8_t ch)
{
	font_t font;
	int i;

	font.gfx = GFX_LoadGFX(filename);
	font.charsize = (cw << 8) + ch;
	font.offset = malloc(256*sizeof(uint16_t));
	font.size = malloc(256*sizeof(uint16_t));

	for (i = 0; i < 256; i++)
		font.offset[i] = 0;
	for (i = 0; i < 256; i++)
		font.size[i] = (cw << 8) + ch;
	return font;
}

void FNT_StartFontEdit(const char *gfx_name, const char *font_name)
{
    if (gfx_name == NULL && font_name == NULL)
    {
        font_edit = true;
        file_menu = true;
        curchar = 0;
		temp_font.charsize = 0;
        DF_UpdateDirfiles(&font_dirfiles, va("%sdata/fonts", I_GetHomeDir()));
        return;
    }

	if (font_name == NULL) {
		temp_font = FNT_CreateFontFromGfx(va("%sdata/fonts/%s.bmp", I_GetHomeDir(), gfx_name), 8, 8);
		sprintf(temp_font_name, gfx_name);
		sprintf(temp_gfx_name, gfx_name);
	} else {
		temp_font = FNT_LoadFont(va("%sdata/fonts/%s.fnt", I_GetHomeDir(), font_name));
		sprintf(temp_font_name, font_name);
	}

	curchar = 0;
	file_menu = false;
    font_edit = true;
}

void FNT_FontEditUpdate(void)
{
	int8_t lr, ud, xoff, yoff, txoff, tyoff;
	uint8_t w, h, charw, charh, tw, th, tcharw, tcharh, oldchar;
	uint16_t tsize, tcharoffset, tcharsize;

	if (file_menu) {
		if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
            file_menu = false;
            return;
        }

        if (font_dirfiles.num_files < 1)
            return; // no files so no loading anything

        if (G_ControlDown(PLAYER_ONE, CON_UP, true))
            curchar--;
        if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
            curchar++;
        if (G_ControlDown(PLAYER_ONE, CON_LEFT, true))
            curchar-=16;
        if (G_ControlDown(PLAYER_ONE, CON_RIGHT, true))
            curchar+=16;

        // sanity checks
        if (curchar < 0)
            curchar = font_dirfiles.num_files - abs(curchar);

        // still below zero?
        if (curchar < 0)
            curchar = 0; // sigh..
        
        // loop around if need be
        curchar = curchar % font_dirfiles.num_files;

        if (G_ControlDown(PLAYER_ONE, CON_A, true)) {
            char *dot;
            char *name;
            const char *filename;
            int name_len;

            filename = font_dirfiles.filenames[curchar];
            dot = strrchr(filename, '.');

            name_len = strlen(filename) + 1;
            name = malloc((name_len-4) * sizeof(char)); 
            snprintf(name, name_len-4, "%s", filename);

            if (!strcmp(dot, ".bmp") || !strcmp(dot, ".BMP")) {
                FNT_StartFontEdit(name, NULL);
            } else {
                FNT_StartFontEdit(NULL, name);
            }

            free(name);
        }

        // return early
        return;
    }

	if (G_ControlDown(PLAYER_ONE, CON_START, true)) {
		file_menu = true;
		return;
	}

	if (temp_font.charsize <= 0)
	    return;

	lr = G_ControlDown(PLAYER_ONE, CON_RIGHT, true) - G_ControlDown(PLAYER_ONE, CON_LEFT, true);
	ud = G_ControlDown(PLAYER_ONE, CON_DOWN, true) - G_ControlDown(PLAYER_ONE, CON_UP, true);
	
	txoff = xoff = (int8_t)(temp_font.offset[curchar] >> 8);
	tyoff = yoff = (int8_t)(temp_font.offset[curchar] & 0xFF);

	tw = w = temp_font.size[curchar] >> 8;
	th = h = temp_font.size[curchar] & 0xFF;
	tcharw = charw = temp_font.charsize >> 8;
	tcharh = charh = temp_font.charsize & 0xFF;
	oldchar = curchar;

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
	
	if (G_ControlDown(PLAYER_ONE, CON_SELECT, true))
		FNT_SaveTempFont();
	
	if (curchar < 0)
		curchar = 0;
	else if (curchar > 255)
		curchar = 255;
	
	tsize = (tw << 8) + th;
	tcharoffset = ((uint8_t)(txoff) << 8) + (uint8_t)(tyoff);
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

static uint16_t rainbowww(uint32_t counter) {
    uint8_t pos = counter % 256; 
    uint8_t r, g, b;

    if (pos < 85) {
        r = 255 - (pos * 3);
        g = pos * 3;
        b = 0;
    } else if (pos < 170) {
        pos -= 85;
        r = 0;
        g = 255 - (pos * 3);
        b = pos * 3;
    } else {
        pos -= 170;
        r = pos * 3;
        g = 0;
        b = 255 - (pos * 3);
    }

    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void FNT_FontEditDraw(void)
{
	uint8_t charw, charh, w, h;
	uint16_t col;
	int8_t xoff, yoff;
	int i, y, x;

	if (file_menu) {
        V_DrawText("Nozomi Engine Font Editor\nSelect a file to open:", 0, 0, 0);

        if (font_dirfiles.num_files > 0) {
            for (i = 0; i < font_dirfiles.num_files; i++)
                V_DrawText(va("%s", font_dirfiles.filenames[i]), (i/17) * 80 + 8, (i%17) * 10 + 20, 0);

            V_DrawText(">", (curchar/17) * 80 + 1, (curchar%17) * 10 + 20, 0);
        } else
            V_DrawText("No files found.", 8, 20, 0);

        V_DrawText("Close the File Menu with Start/Enter", 0, VID_HEIGHT-8, 0);
        // return early
        return;
    }

    if (temp_font.charsize <= 0) {
        V_DrawText("No font loaded.\nRe-open the File Menu with Start/Enter.", 0, 0, 0);
        return;
    }

	col = rainbowww(I_GetTicks()); 
	charw = temp_font.charsize >> 8;
	charh = temp_font.charsize & 0xFF;

	for (i = 0; i < 256; i++)
		V_DrawTextFromFont(temp_font, va("%c", i), (i % 16) * charw, (i / 16) * charh, 0);
	
	for (y = 0; y < charh-1; y++)
		V_DrawDot((curchar % 16) * charw + charw/2-1, (curchar / 16) * charh + y, col, 0);
	for (x = 0; x < charw-1; x++)
		V_DrawDot((curchar % 16) * charw + x, (curchar / 16) * charh + charh/2-1, col, 0);
	
	xoff = (int8_t)(temp_font.offset[curchar] >> 8);
	yoff = (int8_t)(temp_font.offset[curchar] & 0xFF);
	w = temp_font.size[curchar] >> 8;
	h = temp_font.size[curchar] & 0xFF;
	
	for (x = 0; x < charw*2 + 2; x++) {
		V_DrawDot(VID_WIDTH - charw*4 - 1 + x, 2*charh - 1, col, 0);
		V_DrawDot(VID_WIDTH - charw*4 - 1 + x, 2*charh + 2*charh, col, 0);
	}
	
	for (y = 0; y < charh*2 + 2; y++) {
		V_DrawDot(VID_WIDTH - charw*4 - 1, 2*charh-1 + y, col, 0);
		V_DrawDot(VID_WIDTH - charw*4 + charw*2, 2*charh-1 + y, col, 0);
	}

	V_DrawText(va("Offsets: %d, %d", xoff, yoff), 0, charw*charh+charh, 0);
	V_DrawText(va("Char Size (wxh): %d, %d", w, h), 0, charw*charh+charh*2, 0);
	V_DrawText(va("Cell Size (wxh): %d, %d", charw, charh), 0, charw*charh+charh*3, 0);
	
	V_DrawTextFromFont(temp_font, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\nZA 01234567890", 0, VID_HEIGHT - charh*6 - 2, 0);
	V_DrawTextFromFont(temp_font, "abcdefghijklmnopqrstuvwxyz\nza .,/\\;:\'\"[]{}-_=+!@#$%^&*()~|", 0, VID_HEIGHT - charh*4 - 2, 0);
	V_DrawTextFromFont(temp_font, "The quick brown fox\njumped over the lazy dog.", 0, VID_HEIGHT - charh*2 - 2, 0);
}

void FNT_SaveTempFont(void)
{
	FILE *fp = fopen(va("%sdata/fonts/%s.fnt", I_GetHomeDir(), temp_font_name), "wb+");
	char gfx_name[33];
	int i;

	snprintf(gfx_name, 32, temp_gfx_name);
	fwrite(gfx_name, sizeof(gfx_name)-1, 1, fp);
	fwrite(&temp_font.charsize, sizeof(uint16_t), 1, fp);
	
	for (i = 0; i < 256; i++)
		fwrite(&temp_font.offset[i], sizeof(uint16_t), 1, fp);
	for (i = 0; i < 256; i++)
		fwrite(&temp_font.size[i], sizeof(uint16_t), 1, fp);

	fclose(fp);
}
