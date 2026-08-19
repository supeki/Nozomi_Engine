// Nozomi Engine
// game_gfx.h

#ifndef GAME_GFX_H
#define GAME_GFX_H

#include "game_defs.h"

typedef struct
{
	int32_t width;  // image width
	int32_t height; // image height
	uint16_t *data;	// pixel data
} gfx_t;

void GFX_InitGFX(void);

gfx_t GFX_LoadGFX(const char *filename);
gfx_t GFX_LoadLegacyGFX(const char *filename);

extern gfx_t gfx_her, gfx_tiles, gfx_tree, gfx_tree2, gfx_tree3, gfx_textbox;

#endif
