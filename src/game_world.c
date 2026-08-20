// Nozomi Engine
// game_world.c

#include "game_world.h"

uint8_t tile_width, tile_height; // 256x256 is pretty big for one tile as-is
uint32_t *tile_attributes; // per-tile attributes (basically just flags)

uint16_t world_width, world_height; // shouldn't need larger than 65536x65536 tiles right
uint32_t world_bgtype; // background type maybe if i wanna have a 2d game with cool backgrounds (like cave story)
uint16_t *world_tiles; // tile layout in the world