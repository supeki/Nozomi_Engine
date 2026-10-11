// Nozomi Engine
// game_3dworld.h

#ifndef GAME_3DWORLD_H
#define GAME_3DWORLD_H

#include "game_defs.h"
#include "game_gfx.h"

typedef struct
{
	int32_t x, y, z; // x, y, z coordinate
	int32_t tx, ty; // texture x and y
} vertex_t;

typedef struct
{
	vertex_t vertices[3];
	int32_t normal[3];
} triangle_t;

typedef struct
{
	uint32_t num_tris;
	triangle_t *tris;
	gfx_t texture;
} mesh_t;

#endif // GAME_3DWORLD_H
