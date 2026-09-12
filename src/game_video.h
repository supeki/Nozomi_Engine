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
void V_Free(void);
void V_LoadPalette(const char *filename, uint16_t* pal);
uint16_t V_MixColors(uint16_t c, uint16_t c2, uint8_t a);
void V_FillScreen(uint16_t col);
void V_DrawDot(int16_t x, int16_t y, uint16_t col);
void V_DrawLine(int16_t x, int16_t y, int32_t angle, uint16_t length, uint16_t col);
void V_DrawBox(int16_t x, int16_t y, int32_t angle, uint16_t width, uint16_t height, uint16_t col);
void V_DrawTiled(gfx_t gfx, int16_t x, int16_t y, uint32_t flags);
void V_Draw(gfx_t gfx, int16_t x, int16_t y, uint32_t flags);
void V_DrawCroppedAnimated(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t frames, uint32_t fps, uint32_t flags);
void V_DrawCropped2x(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t flags);
void V_DrawCropped(gfx_t gfx, int16_t x, int16_t y, int16_t sx, int16_t sy, uint16_t w, uint16_t h, uint32_t flags);
void V_DrawTextFromFont(font_t font, const char* string, int16_t x, int16_t y, uint32_t flags); // draw text from a font
void V_DrawText(const char* string, int16_t x, int16_t y, uint32_t flags); // use default font (MUST EXIST ALWAYS)

typedef enum
{
	V_WAVYTEXT = 1,
	V_WAVYTEXTTWO = 2,
	V_JUMPYTEXT = 4,
	V_SMALL = 8,
	V_HALFTRANS = 16,
	NUMVIDEOFLAGS
} video_flags_e;

#endif
