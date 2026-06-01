// JADEFRACTURE
// NDS backend
// nds_video.h

#include <gl2d.h>
#include "../../game_gfx.h"

#ifndef NDS_VIDEO_H
#define NDS_VIDEO_H

uint16_t rgb565_to_rgb15(uint16_t c);

void NDS_DrawDot(int x, int y, uint8_t col);
void NDS_DrawCroppedSprite(gfx_t gfx, int x, int y, int sx, int sy, int w, int h);

#endif
