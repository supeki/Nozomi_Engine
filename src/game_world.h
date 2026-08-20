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
    TILE_WATER = 2, // water tile (draw with wavy effect and has water properties with objects)
    TILE_BG = 4, // background tile (draw behind object)
    TILE_FG = 8 // foreground tile (draw in front of object)
} tileattr_e;

typedef enum
{
    BG_NONE = 0,
    BG_TILE = 1, // use tiles for background (doesn't apply water wavy effect on water tiles)
    BG_STATIC = 2, // fixed on-screen (doesn't move)
    BG_FOLLOW = 4, // follow camera (inverse)
    BG_WATER = 8, // use water wavy effect on background (applies to full background)
    BG_FG = 16 // place background in front of tiles (foreground)
} bgattr_e;

extern uint8_t tile_width, tile_height; // 256x256 is pretty big for one tile as-is
extern uint32_t *tile_attributes; // per-tile attributes (basically just flags)

extern uint16_t world_width, world_height; // shouldn't need larger than 65536x65536 tiles right
extern uint32_t world_bgtype; // background type maybe if i wanna have a 2d game with cool backgrounds (like cave story)
extern uint16_t *world_tiles; // tile layout in the world

#endif
