// JADEFRACTURE
// game_gfx.h

#ifndef GAME_GFX_H
#define GAME_GFX_H

#include "game_defs.h"

#define PAL_INDEX(i) (i >> 1)
#define ALPHA_INDEX(i) (i & 1)

typedef struct
{
	uint16_t size;
	uint8_t width;
	int8_t xoff;
	int8_t yoff;
	uint8_t *data;
} gfx_t;

void GFX_InitGFX(void);
gfx_t GFX_LoadGFX(const char *filename);

extern gfx_t gfx_characters;

#endif