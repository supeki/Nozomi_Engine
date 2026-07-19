// Nozomi Engine
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"
#include "game_gfx.h"

typedef enum
{
	TILE_NONE = 0,
	TILE_SOLID = 1,
	TILE_SEMI = 2,
	TILE_CRUMBLE = 4,
	TILE_WATER = 8,
	TILE_MOVING = 16
} tiletype_e;

typedef struct tile_s
{
	struct tile_s *prev, *next;
	
	uint32_t id;
	subpixel_t x, y; // position in the world
} tile_t;

typedef struct tileattr_s
{
	uint32_t num_tiles, tiles_per_row, *type;
	uint8_t *layer;
} tileattr_t;

extern tile_t tiles;
extern tileattr_t tileattr;
extern uint16_t world_width, world_height;

void W_InitTiles(void);
void W_LoadWorld(const char* filename);
void W_HandleTiles(void);
tile_t *W_CreateTile(subpixel_t x, subpixel_t y, uint8_t id);
void W_RemoveTile(tile_t *tile);
void W_FreeWorld(void);
void W_SaveWorld(const char* filename);
void W_DrawTileLayer(uint8_t layer);
void W_MakeTileAttrFromGfx(const char *filename, gfx_t gfx, uint16_t cell_size);
void W_DrawWaterTiles(void);

#endif
