// Nozomi Engine
// X11 backend
// x11_event.c

#include "x11_main.h"

#include "../../i_event.h"
#include "../../i_video.h"

#include "../../game_defs.h"
#include "../../game_main.h"
#include "../../game_input.h"

void I_PollEvents(void)
{
	XEvent event;

	while (XPending(Disp)) {
		XNextEvent(Disp, &event);

		switch (event.type) {
			case Expose:
			{
				I_PushGraphics();
				break;
			}

			case ClientMessage:
			{
				if ((Atom)event.xclient.data.l[0] == DeleteMsg)
					game_quit = true;

				break;
			}

			case ConfigureNotify:
			{
				// TO-DO: Add window resizing Nozomi 02-NOV-26
				break;
			}

			case KeyPress:
			case KeyRelease:
			{
				KeySym sym;
				bool pressed = (event.type == KeyPress);

				// filter out the auto repeat mrrp
				if (XEventsQueued(Disp, QueuedAfterReading)) {
					XEvent next_event;
					XPeekEvent(Disp, &next_event);

					if (!pressed && next_event.type == KeyPress && next_event.xkey.keycode == event.xkey.keycode && next_event.xkey.time == event.xkey.time) {
						XNextEvent(Disp, &next_event);
						continue;
					}
				}

				sym = XLookupKeysym(&event.xkey, 0);

				if (pressed) {
					if (sym == XK_Escape) // until exiting is implemented in a menu or something
						game_quit = true;

					for (uint8_t c = 0; c < NUMCONTROLS; c++)
						if (gamecontrolbinds[0][c][0] == sym)
							gamecontrols[0][c]++;
				} else
					for (uint8_t c = 0; c < NUMCONTROLS; c++)
						if (gamecontrolbinds[0][c][0] == sym)
							gamecontrols[0][c] = 0;
				break;
			}
		}
	}

	mouseaxis[0][MOUSE_POSX] = 0;
	mouseaxis[0][MOUSE_POSY] = 0;
	mouseaxis[0][MOUSE_INPUTX] = 0;
	mouseaxis[0][MOUSE_INPUTY] = 0;
	mousecontrols[0][MOUSE_LBUTTON] = 0;
}
