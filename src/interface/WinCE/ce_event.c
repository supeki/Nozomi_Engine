// Nozomi Engine
// WinCE backend
// ce_event.c

#include "ce_main.h"
#include "ce_input.h"

#include "../../i_event.h"
#include "../../game_input.h"
#include "../../game_main.h"

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
		}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	}
}
