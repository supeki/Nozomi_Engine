// JADEFRACTURE
// game_video.h

#include "game_defs.h"
#include "game_gfx.h"

#define BASEVIDWIDTH 256
#define BASEVIDHEIGHT 160

#ifndef GAME_VIDEO_H
#define GAME_VIDEO_H

typedef struct
{
	int width; // Screen Width
	int height; // Screen Height
	uint8_t* buffer; // Framebuffer
} vid_t;

extern vid_t vid;
extern uint16_t* palette;

void V_Init(void);
void V_LoadPalette(void);
void V_DrawDot(int x, int y, byte col);
void V_DrawSprite(int x, int y, gfx_t gfx);

#endif