// Nozomi Engine
// game_video.h

#ifndef GAME_VIDEO_H
#define GAME_VIDEO_H

#include "game_defs.h"
#include "game_gfx.h"

#define BASEVIDWIDTH 256
#define BASEVIDHEIGHT 192

typedef struct
{
	int width; // Screen Width
	int height; // Screen Height
	uint16_t* buffer; // Framebuffer
} vid_t;

extern vid_t vid;
extern uint16_t* palette;

void V_Init(void);
void V_LoadPalette(void);
void V_ClearScreen(void);
void V_DrawDot(int x, int y, uint8_t col);
void V_Draw(gfx_t gfx, int x, int y);
void V_DrawCropped(gfx_t gfx, int x, int y, int sx, int sy, int w, int h);
void V_DrawText(const char* string, int x, int y, int flags);

void V_DrawCroppedBitmap(bitmap_gfx_t gfx, int x, int y, int sx, int sy, int w, int h);

#endif
