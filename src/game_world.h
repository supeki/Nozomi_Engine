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
    BG_NONE = 0, // no background
    BG_STATIC = 1, // fixed on-screen (doesn't move)
    BG_WATER = 2, // use water wavy effect on background (applies to layer 1)
    BG_FG = 4, // place layer 1 background in front of tiles (foreground layer!!!??)
    BG_SLOW = 8, // move slower than the tiles (parallax-ish) (layer 2 moves slower than layer 1)
    BG_COLOR = 16, // use a solid color for the background
    BG_IMAGE = 32, // use an image for the background as well
    BG_SCROLL = 64 // scroll the two tile layers to the left automatically (clouds maybe) (it will loop the bg don't worry)
} bgattr_e;

#define BG_OPTIONS 7

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
void W_SaveWorldFile(const char *filename);
void W_Free(void);

void W_CreateTilesetFromFile(const char *input, uint8_t tile_size);
void W_CreateWorldFromTilesetFile(const char *input, uint16_t width, uint16_t height);

void W_DrawLayer(uint8_t layer);
void W_DrawWaveEffect(uint16_t color);
void W_DrawWorld(void);

extern bool world_edit;
void W_StartWorldEdit(const char *gfx_name, const char *tileset_name);
void W_UpdateWorldEdit(void);
void W_DrawWorldEdit(void);

extern bool tileset_edit;
void W_StartTilesetEdit(const char *gfx_name, const char *tileset_name);
void W_UpdateTilesetEdit(void);
void W_DrawTilesetEdit(void);

#endif
