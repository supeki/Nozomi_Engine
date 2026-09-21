// Nozomi Engine
// Commodore Amiga backend
// amiga_video.c

#include "amiga_main.h"
#include "../../i_video.h"
#include "../../i_system.h"

#include "../../game_defs.h"
#include "../../game_video.h"

#include <exec/types.h>
#include <exec/memory.h>
#include <exec/libraries.h>

#include <intuition/intuition.h>
#include <graphics/gfx.h>
#include <graphics/view.h>
#include <graphics/rastport.h>

#include <proto/exec.h>
#include <proto/intuition.h>
#include <proto/graphics.h>

static struct Screen *amiga_screen = NULL;
struct Window *amiga_window = NULL;
static struct BitMap amiga_bitmaps[2];
static uint8_t *amiga_planes[2][5];
static uint8_t current_buffer = 0;

static uint16_t amiga_palette[32];
static uint8_t amiga_color_lut[4096];
static uint8_t rgb565_to_amiga_lut[65536];

// loads special 32-color palette for amiga and makes LUTs for the framebuffer to use
// takes like 8 seconds
static void create_amiga_pal(void)
{
	int i;
	FILE *file = fopen(va("%sdata/palette.ami", I_GetHomeDir()), "rb");
	uint8_t rdist[16][16];
	uint8_t gdist[16][16];
	uint8_t bdist[16][16];

	if (file == NULL)
		I_Error("Failed to read palette file! (amiga)\n");

	for (i = 0; i < 32; i++)
	{
		uint8_t r, g, b;

		fread(&r, 1, 1, file);
		fread(&g, 1, 1, file);
		fread(&b, 1, 1, file);

		r >>= 4;
		g >>= 4;
		b >>= 4;

		amiga_palette[i] = (r << 8) | (g << 4) | b;
	}

	for (i = 0; i < 32; i++)
	{
		SetRGB4(
			&amiga_screen->ViewPort,
			i,
			amiga_palette[i] >> 8,
			(amiga_palette[i] >> 4) & 0xF,
			amiga_palette[i] & 0xF
		);
	}

	for (i = 0; i < 16; i++)
	{
		int j;

		for (j = 0; j < 16; j++)
		{
			int d = i - j;

			rdist[i][j] = d * d * 2;
			gdist[i][j] = d * d * 4;
			bdist[i][j] = d * d * 3;
		}
	}

	for (int c = 0; c < 4096; c++)
	{
		int r = (c >> 8) & 0xF;
		int g = (c >> 4) & 0xF;
		int b = c & 0xF;

		int best_idx = 0;
		int min_dist = 0x7FFF;

		for (i = 0; i < 32; i++)
		{
			int pr = (amiga_palette[i] >> 8) & 0xF;
			int pg = (amiga_palette[i] >> 4) & 0xF;
			int pb = amiga_palette[i] & 0xF;

			int dist =
				rdist[r][pr] +
				gdist[g][pg] +
				bdist[b][pb];

			if (dist < min_dist)
			{
				min_dist = dist;
				best_idx = i;

				if (dist == 0)
					break;
			}
		}

		amiga_color_lut[c] = (uint8_t)best_idx;
	}

	for (int c = 0; c < 65536; c++)
	{
		int r = (c >> 12) & 0xF;
		int g = (c >> 7) & 0xF;
		int b = (c >> 1) & 0xF;

		rgb565_to_amiga_lut[c] =
			amiga_color_lut[(r << 8) | (g << 4) | b];
	}

	fclose(file);
}

void I_StartupGraphics(void)
{
	struct NewScreen ns;
	struct NewWindow nw;
	int i, b, p;

	// two buffers
	for (b = 0; b < 2; b++)
	{
		InitBitMap(&amiga_bitmaps[b], 5, 256, 200);
		
		for (p = 0; p < 5; p++)
		{
			amiga_planes[b][p] = AllocRaster(256, 200);
			if (amiga_planes[b][p] == NULL)
				I_Error("Failed to allocate plane buffers!");
				
			amiga_bitmaps[b].Planes[p] = amiga_planes[b][p];			
			memset(amiga_planes[b][p], 0, RASSIZE(256, 200));
		}
	}

	memset(&ns, 0, sizeof(ns));
	ns.LeftEdge = 0;
	ns.TopEdge = 0;
	ns.Width = 256;
	ns.Height = 200;
	ns.Depth = 5;
	ns.DetailPen = 0;
	ns.BlockPen = 0;
	ns.ViewModes = 0;
	ns.Type = CUSTOMSCREEN | CUSTOMBITMAP; // double buffer
	ns.Font = NULL;
	ns.DefaultTitle = NULL;
	ns.Gadgets = NULL;
	ns.CustomBitMap = &amiga_bitmaps[0];

	amiga_screen = OpenScreen(&ns);
	if (amiga_screen == NULL)
		I_Error("Failed to open Amiga screen!\n");

	create_amiga_pal();

	// window stuffs
	memset(&nw, 0, sizeof(nw));
	nw.LeftEdge = 0;
	nw.TopEdge = 0;
	nw.Width = 256;
	nw.Height = 200;
	nw.DetailPen = 0;
	nw.BlockPen = 0;
	nw.IDCMPFlags = IDCMP_RAWKEY | IDCMP_MOUSEBUTTONS | IDCMP_MOUSEMOVE; // for input
	nw.Flags = WFLG_BORDERLESS | WFLG_BACKDROP | WFLG_REPORTMOUSE | WFLG_RMBTRAP | WFLG_ACTIVATE; // take the whole screen
	nw.FirstGadget = NULL;
	nw.CheckMark = NULL;
	nw.Title = NULL;
	nw.Screen = amiga_screen;
	nw.BitMap = NULL;
	nw.MinWidth = 256;
	nw.MinHeight = 200;
	nw.MaxWidth = 256;
	nw.MaxHeight = 200;
	nw.Type = CUSTOMSCREEN;

	amiga_window = OpenWindow(&nw);
	if (amiga_window == NULL) {
		CloseScreen(amiga_screen);
		amiga_screen = NULL;

		I_Error("Failed to open Amiga window!");
	}

    mouse_offx = (256*(VID_WIDTH/256) - VID_WIDTH) / 2;
    mouse_offy = (200*(VID_HEIGHT/192) - VID_HEIGHT) / 2;
}

void I_ShutdownGraphics(void)
{
	int b, p;

	if (amiga_window != NULL) {
		CloseWindow(amiga_window);
		amiga_window = NULL;
	}

	if (amiga_screen != NULL) {
		CloseScreen(amiga_screen);
		amiga_screen = NULL;
	}

	for (b = 0; b < 2; b++) {
		for (p = 0; p < 5; p++) {
			if (amiga_planes[b][p] != NULL) {
				FreeRaster(amiga_planes[b][p], 256, 200);
				amiga_planes[b][p] = NULL;
			}
		}
	}
}

// this is probably the best it will get
// oh my god it's so fucking slow (7 seconds per frame)
// i don't know how to speed it up more
void I_PushGraphics(void)
{
	int x, y;
	uint16_t *src;
	uint8_t *p0, *p1, *p2, *p3, *p4;
	uint8_t a, b, c, d, e, f, g, h;

	if (amiga_screen == NULL)
		return;

	// thank god i know how to flip bits
	int back_buffer = current_buffer ^ 1; 
	uint16_t bytes_per_row = amiga_bitmaps[back_buffer].BytesPerRow;

	for (y = 0; y < VID_HEIGHT; y++)
	{
		src = &vid.buffer[y * VID_WIDTH];

		p0 = &amiga_planes[back_buffer][0][y * bytes_per_row];
		p1 = &amiga_planes[back_buffer][1][y * bytes_per_row];
		p2 = &amiga_planes[back_buffer][2][y * bytes_per_row];
		p3 = &amiga_planes[back_buffer][3][y * bytes_per_row];
		p4 = &amiga_planes[back_buffer][4][y * bytes_per_row];

		for (x = 0; x < VID_WIDTH; x += 8)
		{
			a = rgb565_to_amiga_lut[src[0]];
			b = rgb565_to_amiga_lut[src[1]];
			c = rgb565_to_amiga_lut[src[2]];
			d = rgb565_to_amiga_lut[src[3]];
			e = rgb565_to_amiga_lut[src[4]];
			f = rgb565_to_amiga_lut[src[5]];
			g = rgb565_to_amiga_lut[src[6]];
			h = rgb565_to_amiga_lut[src[7]];

			// i rember seeing a doom rendering function do something like this before
			p0[0] = ((a & 1) << 7) | ((b & 1) << 6) | ((c & 1) << 5) | ((d & 1) << 4) |
			        ((e & 1) << 3) | ((f & 1) << 2) | ((g & 1) << 1) | (h & 1);

			p1[0] = ((a & 2) << 6) | ((b & 2) << 5) | ((c & 2) << 4) | ((d & 2) << 3) |
			        ((e & 2) << 2) | ((f & 2) << 1) | (g & 2)        | ((h & 2) >> 1);

			p2[0] = ((a & 4) << 5) | ((b & 4) << 4) | ((c & 4) << 3) | ((d & 4) << 2) |
			        ((e & 4) << 1) | (f & 4)        | ((g & 4) >> 1) | ((h & 4) >> 2);

			p3[0] = ((a & 8) << 4) | ((b & 8) << 3) | ((c & 8) << 2) | ((d & 8) << 1) |
			        (e & 8)        | ((f & 8) >> 1) | ((g & 8) >> 2) | ((h & 8) >> 3);

			p4[0] = ((a & 16) << 3) | ((b & 16) << 2) | ((c & 16) << 1) | (d & 16) |
			        ((e & 16) >> 1) | ((f & 16) >> 2) | ((g & 16) >> 3) | ((h & 16) >> 4);

			src += 8; p0++; p1++; p2++; p3++; p4++;
		}
	}

	current_buffer = back_buffer;
	amiga_screen->ViewPort.RasInfo->BitMap = &amiga_bitmaps[current_buffer];
	
	WaitTOF(); 
	ScrollVPort(&amiga_screen->ViewPort);
}
