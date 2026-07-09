// Nozomi Engine
// game_font.c

#include "game_font.h"

font_t FNT_LoadFont(const char *filename)
{
	FILE *fp = fopen(filename, "r");
	font_t font;
	char gfx_name[37]; // 32, 4 for extension, 1 null byte

	fread(&font.unicode, sizeof(bool), 1, fp);
	fread(&font.bitmap, sizeof(bool), 1, fp);
	fread(gfx_name, sizeof(char), 32, fp);

	if (font.bitmap)
		font.bmpgfx = BMPGFX_LoadBitmap(gfx_name);
	else
		font.gfx = GFX_LoadGFX(gfx_name);

	return font;
}
