// Nozomi Engine
// WinCE backend
// ce_event.c

#include "ce_main.h"
#include "ce_input.h"

#include "../../i_event.h"
#include "../../game_input.h"
#include "../../game_main.h"

uint16_t old_mouseposx = 0, old_mouseposy = 0;
int mouse_offx, mouse_offy;

void I_PollEvents(void)
{
    MSG msg;

    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
		switch (msg.message) {
			case WM_QUIT:
			case WM_CLOSE:
			case WM_DESTROY:
				game_quit = true;
				continue;
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				int res = 1, j, c;
				if (msg.message == WM_KEYUP)
					res = 0;
				else if (msg.lParam & 0x40000000)
					continue;

				for (j = 0; j < MAX_PLAYERS; j++)
					for (c = CON_UP; c < NUMCONTROLS; c++)
						if (gamecontrolbinds[j][c][0] == msg.wParam) // holy fucking shit
							gamecontrols[j][c] = res;
				continue;
			}
			case WM_MOUSEMOVE:
			{
				mouseaxis[0][MOUSE_POSX] = 320 - HIWORD(msg.lParam) - 1 - mouse_offx;
				mouseaxis[0][MOUSE_POSY] = LOWORD(msg.lParam) - mouse_offy;
				mouseaxis[0][MOUSE_INPUTX] = mouseaxis[0][MOUSE_POSX] - old_mouseposx;
				mouseaxis[0][MOUSE_INPUTY] = mouseaxis[0][MOUSE_POSY] - old_mouseposy;
				old_mouseposx = mouseaxis[0][MOUSE_POSX];
				old_mouseposy = mouseaxis[0][MOUSE_POSY];
				continue;
			}
			case WM_LBUTTONDOWN: // weirdly these fire first on Windows CE, which can cause me issues,,
			case WM_LBUTTONUP: // issues that I don't know how to fix right now
			{
				int res = 1;
				if (msg.message == WM_LBUTTONUP)
					res = 0;

				mousecontrols[0][MOUSE_LBUTTON] = res;
				continue;
			}
		}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
}
