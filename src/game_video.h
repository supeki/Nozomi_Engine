// JADEFRACTURE
// game_video.h

#include "game_defs.h"

#define BASEVIDWIDTH 192
#define BASEVIDHEIGHT 128

#ifndef GAME_VIDEO_H
#define GAME_VIDEO_H

typedef struct
{
	byte* buffer; // Framebuffer
	int width; // Screen Width
	int height; // Screen Height
} vid_t;

extern vid_t vid;

void V_Init(void);
void V_DrawDot(int x, int y, byte col);

#endif