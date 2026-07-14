// Nozomi Engine
// NDS backend
// nds_video.c

#include <nds.h>
#include "nds_main.h"

#include "../../i_video.h"
#include "../../game_video.h"

void I_StartupGraphics(void)
{	
	//vramDefault();
	videoSetMode(MODE_VRAM_A);
    vramSetBankA(VRAM_A_LCD);
}

uint16_t rgb565_to_rgb15(uint16_t c)
{
    uint16_t r = (c >> 11) & 0x1F;
    uint16_t g = (c >> 5) & 0x3F;
    uint16_t b =  c & 0x1F;
	g = g >> 1;
    return RGB15(r, g, b);
}

void I_PushGraphics(void)
{
	uint16_t* pixels = (uint16_t*)VRAM_A;

	for (int i = 0; i < VID_WIDTH * VID_HEIGHT; i++)
	{
		pixels[i] = rgb565_to_rgb15(vid.buffer[i]);
	}
}
