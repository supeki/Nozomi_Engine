// JADEFRACTURE
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"

typedef struct tile_s
{
	struct tile_s *prev; // previous tile
	struct tile_s *next; // next tile

	subpixel_t x; // dynamic tiles? :eyes:
	subpixel_t y; // :fox:
} tile_t;

typedef struct
{
	gfx_t gfx; // tile graphics
	uint8_t *data; // tile types 
} tileset_t;

typedef struct
{
	tileset_t tileset;
	tile_t tiles;
} world_t;

#endif