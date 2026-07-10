// Nozomi Engine
// game_gfx.h

#ifndef GAME_GFX_H
#define GAME_GFX_H

#include "game_defs.h"

typedef struct
{
	uint32_t size; // updated to uint32_t Nozomi 22-JUN-26 // apparently it never actually got done or something Nozomi 08-JUL-26
	uint16_t width; // updated to uint16_t Nozomi 22-JUN-26
	int16_t xoff; // updated to int16_t Nozomi 22-JUN-26
	int16_t yoff; // updated to int16_t Nozomi 22-JUN-26
	uint8_t *data;
} gfx_t;

typedef struct
{
	int32_t width;  // image width
	int32_t height; // image height
	uint16_t *data;	// pixel data
} bitmap_gfx_t;

void GFX_InitGFX(void);
gfx_t GFX_LoadGFX(const char *filename);

bitmap_gfx_t BMPGFX_LoadBitmap(const char *filename);

#endif
