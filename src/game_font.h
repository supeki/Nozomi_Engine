// Nozomi Engine
// game_font.h

#ifndef GAME_FONT_H
#define GAME_FONT_H

#include "game_gfx.h"

typedef struct
{
	bool unicode;        // if true, expect utf-8 characters,
	bool bitmap;	     // use bitmap format instead of gfx format
	gfx_t gfx;           // else expect extended ascii
	bitmap_gfx_t bmpgfx; // bmp is supported (without color-changing)
	int32_t *offset;     // upper 16 bits - x, lower 16 bits - y
	uint32_t *size;      // upper 16 bits - w, lower 16 bits - h
} font_t;

font_t FNT_LoadFont(const char *filename);

#endif
