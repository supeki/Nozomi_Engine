// JADEFRACTURE
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"

typedef struct tile_s
{
	uint32_t id; // tileset tile id
	subpixel_t x; // dynamic tiles? :eyes:
	subpixel_t y; // :fox:
} tile_t;

typedef struct
{
	gfx_t gfx; // tile graphics
	uint32_t *data; // tile types 
} tileset_t;

typedef struct
{
	tileset_t tileset;
	tile_t *tiles;
} world_t;

extern world_t world; // za warudo!

void W_InitWorld(void);
tileset_t W_LoadTileset(const char *setname);
void W_LoadWorld(const char *worldname);

#endif
