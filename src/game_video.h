// Nozomi Engine
// game_video.h

#ifndef GAME_VIDEO_H
#define GAME_VIDEO_H

#include "game_defs.h"
#include "game_gfx.h"  // gfx_t, bitmap_gfx_t
#include "game_font.h" // font_t

#define VID_WIDTH 256
#define VID_HEIGHT 192

typedef struct
{
	uint16_t width; // Screen Width
	uint16_t height; // Screen Height
	uint16_t* buffer; // Framebuffer
} vid_t;

extern vid_t vid;
extern uint16_t* palette;

void V_Init(void);
void V_LoadPalette(void);
void V_ClearScreen(void);
void V_DrawDot(int16_t x, int16_t y, uint8_t col);
void V_Draw(gfx_t gfx, int16_t x, int16_t y);
void V_DrawCropped(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h);
void V_DrawTextFromFont(font_t font, const char* string, int16_t x, int16_t y, int flags); // draw text from a font
void V_DrawText(const char* string, int16_t x, int16_t y, int flags); // use default font (MUST EXIST ALWAYS)

void V_DrawBitmap(bitmap_gfx_t, int16_t x, int16_t y);
void V_DrawCroppedBitmap(bitmap_gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h);

#endif
