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

void I_ShutdownGraphics(void)
{			
}

void I_PushGraphics(void)
{
    uint16_t dx, dy;
    uint32_t *dest = dibPixels;

    // our y is now actually our x
    for (dy = 0; dy < VID_WIDTH; dy++)
    {
        uint16_t src_x = VID_WIDTH - 1 - dy;
        
        // and vice-versa
        for (dx = 0; dx < 192; dx++)
        {
            uint16_t src_y = dx;
            uint16_t p = vid.buffer[src_x + src_y * VID_WIDTH];
            uint32_t r = (p >> 11) & 0x1F;
            uint32_t g = (p >> 5)  & 0x3F;
            uint32_t b = p & 0x1F;
            
            r = (r << 3) | (r >> 2);
            g = (g << 2) | (g >> 4);
            b = (b << 3) | (b >> 2);

            *dest++ = (r << 16) | (g << 8) | b;
        }
    }

    InvalidateRect(g_hWnd, NULL, FALSE);
}
