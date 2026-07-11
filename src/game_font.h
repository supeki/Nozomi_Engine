// Nozomi Engine
// game_font.h

#ifndef GAME_FONT_H
#define GAME_FONT_H

#include "game_gfx.h"

typedef struct
{
	bool bitmap;	     // use bitmap format instead of gfx format
	gfx_t gfx;
	bitmap_gfx_t bmpgfx; // bmp is supported (without color-changing)
	uint16_t charsize;   // upper 8 bits - w, lower 8 bits - h 
	int16_t *offset;     // upper 8 bits - x, lower 8 bits - y
	uint16_t *size;      // upper 8 bits - w, lower 8 bits - h
} font_t;

font_t FNT_LoadFont(const char *filename);

extern font_t font_default;
extern bool font_edit;
void FNT_StartFontEdit(void);
void FNT_FontEditUpdate(void);
void FNT_FontEditDraw(void);
void FNT_SaveTempFont(void);

#endif
