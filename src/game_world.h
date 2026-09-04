// Nozomi Engine
// game_world.h

#ifndef GAME_WORLD_H
#define GAME_WORLD_H

#include "game_defs.h"
#include "game_gfx.h"

typedef enum
{
    TILE_NONE = 0,
    TILE_SOLID = 1, // solid tile, objects collide with it
    TILE_WATER = 2, // water tile (has water properties with objects)
    TILE_FG = 4, // foreground tile (draw in front of object)
    TILE_ANIMATED = 8
} tileattr_e;

// TILE_OPTIONS should be the number of items above minus TILE_NONE
#define TILE_OPTIONS 4

typedef enum
{
    BG_NONE = 0,
    BG_TILE = 1, // use tiles for background (doesn't apply water wavy effect on water tiles)
    BG_STATIC = 2, // fixed on-screen (doesn't move)
    BG_FOLLOW = 4, // follow camera (inverse)
    BG_WATER = 8, // use water wavy effect on background (applies to full background)
    BG_FG = 16 // place background in front of tiles (foreground)
} bgattr_e;

// cool tips n' tricks with mareuhlyn
// use BG_FG + BG_WATER for a wavy overlay in underwater areas!

extern uint8_t tile_width, tile_height; // 256x256 is pretty big for one tile as-is
extern uint32_t *tile_attributes; // per-tile attributes (basically just flags)
extern gfx_t gfx_tileset;

extern uint16_t world_width, world_height; // shouldn't need larger than 65536x65536 tiles right
extern uint32_t world_bgtype; // background type maybe if i wanna have a 2d game with cool backgrounds (like cave story)
extern uint16_t *world_tiles, *world_bgtiles; // tile layout in the world
extern gfx_t gfx_worldbg;

void W_LoadWorldFile(const char *filename);
void W_Free(void);

void W_CreateTilesetFromFile(const char *input, uint8_t tile_size);

extern bool tileset_edit;
void W_StartTilesetEdit(const char *gfx_name, const char *tileset_name);
void W_UpdateTilesetEdit(void);
void W_DrawTilesetEdit(void);

#endif
