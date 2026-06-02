// Nozomi Engine
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"
#include "game_gfx.h"

typedef struct tile_s
{
	struct tile_s *prev;
	struct tile_s *next;
	
	uint8_t id; // tileset tile id
	subpixel_t x; // dynamic tiles? :eyes:
	subpixel_t y; // :fox:
	uint8_t layer; // what layer this tile is on (determines a lot of things)
} tile_t;

typedef struct
{
	gfx_t gfx; // tile graphics
	uint8_t *data; // tile types 
} tileset_t;

extern tile_t tiles;
extern tileset_t tileset;

void W_InitWorld(void);
void W_LoadTileset(const char *filename);
void W_LoadWorld(const char *filename);
tile_t *W_CreateTile(subpixel_t x, subpixel_t y, uint8_t id, uint8_t layer);
void W_FreeTiles(void);
void W_FreeTileset(void);
void W_SaveWorld(const char *filename);

#endif
