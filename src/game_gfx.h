// Nozomi Engine
// game_gfx.h

#ifndef GAME_GFX_H
#define GAME_GFX_H

#include "game_defs.h"

typedef struct
{
	uint16_t size; // updated to uint32_t Nozomi 22-JUN-26
	uint8_t width; // updated to uint16_t Nozomi 22-JUN-26
	int16_t xoff; // updated to int16_t Nozomi 22-JUN-26
	int16_t yoff; // updated to int16_t Nozomi 22-JUN-26
	uint8_t *data;
} gfx_t;

typedef struct
{
	int32_t width;  // image width
	int32_t height; // image height
	uint16_t *data;	// pixel data
} bitmap_gfx_t;

typedef struct
{
	gfx_t gfx; // font sheet
	bitmap_gfx_t bmp_gfx; // bitmap version 
	uint8_t char_width; // if a single font character is >256 pixels then wtf...
	uint8_t char_height;
	uint16_t *char_offsets; // per-character x/y offsets, first 8 bits are x, last 8 bits are y
} font_t;

void GFX_InitGFX(void);
gfx_t GFX_LoadGFX(const char *filename);

bitmap_gfx_t BMPGFX_LoadBitmap(const char *filename);

extern gfx_t gfx_characters;
extern bitmap_gfx_t bmpgfx_characters;

#endif
