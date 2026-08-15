// Nozomi Engine
// WinCE backend
// ce_video.c

#include "ce_main.h"

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../game_video.h"
#include "../../game_defs.h"

DWORD dibPixels[VID_WIDTH * VID_HEIGHT];
extern HWND g_hWnd;

HDC hdcWindow = NULL; 

void I_StartupGraphics(void)
{		
	hdcWindow = GetDC(g_hWnd);
}

void I_PushGraphics(void)
{
	int i;

    for (i = 0; i < VID_WIDTH * VID_HEIGHT; ++i)
    {
        uint16_t p = vid.buffer[i];

        uint32_t r = (p >> 11) & 0x1F;
        uint32_t g = (p >> 5)  & 0x3F;
        uint32_t b = p & 0x1F;
        
        r = (r << 3) | (r >> 2);
        g = (g << 2) | (g >> 4);
        b = (b << 3) | (b >> 2);

        dibPixels[i] =
            (r << 16) |
            (g << 8)  |
            b;
    }

	InvalidateRect(g_hWnd, NULL, FALSE);
}
