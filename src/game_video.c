// JADEFRACTURE
// game_video.c

#include "game_video.h"

vid_t vid;

void V_Init(void)
{
	vid.width = BASEVIDWIDTH;
	vid.height = BASEVIDHEIGHT;
	
	vid.buffer = malloc(vid.width*vid.height);
	memset(vid.buffer, 0, vid.width*vid.height);
}

void V_DrawDot(int x, int y, byte col)
{
	vid.buffer[x+(y*BASEVIDWIDTH)] = col;
}