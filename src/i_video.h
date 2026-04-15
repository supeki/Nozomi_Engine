// JADEFRACTURE
// i_video.h

#include "game_defs.h"

#ifndef I_VIDEO_H
#define I_VIDEO_H

typedef struct
{
	byte* buffer; // Framebuffer
	int width; // Screen Width
	int height; // Screen Height
} vid_t;

extern vid_t vid;

void I_StartupGraphics(void);
void I_PushGraphics(void);

#endif