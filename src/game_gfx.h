// JADEFRACTURE
// game_gfx.h

#include "game_defs.h"

#define PAL_INDEX(i) (i >> 1)
#define ALPHA_INDEX(i) (i & 1)

#ifndef GAME_GFX_H
#define GAME_GFX_H

typedef struct
{
	uint16_t size;
	uint8_t width;
	int8_t xoff;
	int8_t yoff;
	uint8_t *data;
} gfx_t;

gfx_t GFX_LoadGFX(const char *filename);

#endif