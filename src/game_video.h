// Nozomi Engine
// game_video.h

#ifndef GAME_VIDEO_H
#define GAME_VIDEO_H

#include "game_defs.h"
#include "game_gfx.h"  // gfx_t, bitmap_gfx_t
#include "game_font.h" // font_t

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
void V_DrawTextWithFont(font_t font, const char* string, int x, int y, int flags); // draw text from a font
void V_DrawText(const char* string, int x, int y, int flags); // use default font (MUST EXIST ALWAYS)

void V_DrawCroppedBitmap(bitmap_gfx_t gfx, int x, int y, int sx, int sy, int w, int h);

#endif
