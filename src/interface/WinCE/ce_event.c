// Nozomi Engine
// WinCE backend
// ce_event.c

#include "ce_main.h"
#include "ce_input.h"

#include "../../i_event.h"
#include "../../game_input.h"
#include "../../game_main.h"

uint16_t old_mouseposx = 0, old_mouseposy = 0;

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
				return;
			case WM_KEYDOWN:
			case WM_KEYUP:
			{
				int res = 1, j, c;
				if (msg.message == WM_KEYUP)
					res = 0;

				for (j = 0; j < MAX_PLAYERS; j++)
					for (c = CON_UP; c < NUMCONTROLS; c++)
						if (gamecontrolbinds[j][c][0] == msg.wParam) // holy fucking shit
							gamecontrols[j][c] = res;
				return;
			}
			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			{
				int res = 1;
				if (msg.message == WM_LBUTTONUP)
					res = 0;

				mousecontrols[0][MOUSE_LBUTTON] = res;
				return;
			}
			case WM_MOUSEMOVE:
			{
				mouseaxis[0][MOUSE_POSX] = LOWORD(msg.lParam);
				mouseaxis[0][MOUSE_POSY] = HIWORD(msg.lParam);
				mouseaxis[0][MOUSE_INPUTX] = LOWORD(msg.lParam) - old_mouseposx;
				mouseaxis[0][MOUSE_INPUTY] = HIWORD(msg.lParam) - old_mouseposy;
				old_mouseposx = LOWORD(msg.lParam);
				old_mouseposy = HIWORD(msg.lParam);
				return;
			}
		}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
}
