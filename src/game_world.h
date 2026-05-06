// JADEFRACTURE
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"

typedef struct tile_s
{
	struct tile_s *prev;
	struct tile_s *next;
	
	uint32_t id; // tileset tile id
	subpixel_t x; // dynamic tiles? :eyes:
	subpixel_t y; // :fox:
	uint8_t layer; // what layer this tile is on (determines a lot of things)
} tile_t;

typedef struct
{
	gfx_t gfx; // tile graphics
	uint32_t *data; // tile types 
} tileset_t;

typedef struct
{
	tile_t *tiles;
	tileset_t tileset;
} world_t;

extern uint32_t num_worlds;
extern world_t *worlds;

void W_InitWorld(void);
tileset_t W_LoadTileset(const char *filename);
void W_LoadWorld(const char *filename);

#endif
