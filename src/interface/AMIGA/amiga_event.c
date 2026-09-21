// Nozomi Engine
// Commodore Amiga backend
// amiga_event.c

#include "amiga_main.h"

#include "../../i_event.h"
#include "../../game_input.h"

#include <exec/types.h>
#include <exec/ports.h>
#include <intuition/intuition.h>
#include <proto/exec.h>
#include <proto/intuition.h>

int mouse_offx, mouse_offy;
uint16_t old_mouseposx = 0, old_mouseposy = 0;

static uint8_t amiga_key_states[128];

void I_PollEvents(void)
{
    struct IntuiMessage *msg;
    int j, c;

    while ((msg = (struct IntuiMessage *)GetMsg(amiga_window->UserPort))) {
        ReplyMsg((struct Message *)msg);

        switch (msg->Class)
        {
            case IDCMP_RAWKEY:
                {
                    uint8_t key_code = msg->Code & 0x7F;
                    uint8_t released = msg->Code & 0x80;

                    if (!released)
                        amiga_key_states[key_code] = 1;
                    else
                        amiga_key_states[key_code] = 0;
                }
                break;

            case IDCMP_MOUSEBUTTONS:
                if (msg->Code == SELECTDOWN)
                    mousecontrols[0][MOUSE_LBUTTON]++;
                else if (msg->Code == SELECTUP)
                    mousecontrols[0][MOUSE_LBUTTON] = 0;
                break;

            case IDCMP_MOUSEMOVE:
                mouseaxis[0][MOUSE_POSX] = msg->MouseX - mouse_offx;
                mouseaxis[0][MOUSE_POSY] = msg->MouseY - mouse_offy;
                break;
        }
    }

    for (j = 0; j < MAX_PLAYERS; j++)
    {
        for (c = CON_UP; c < NUMCONTROLS; c++)
        {
            uint8_t bound_key = gamecontrolbinds[j][c][0];

            if (amiga_key_states[bound_key])
                gamecontrols[j][c]++;
            else
                gamecontrols[j][c] = 0;
        }
    }

    mouseaxis[0][MOUSE_INPUTX] = mouseaxis[0][MOUSE_POSX] - old_mouseposx;
    mouseaxis[0][MOUSE_INPUTY] = mouseaxis[0][MOUSE_POSY] - old_mouseposy;
    old_mouseposx = mouseaxis[0][MOUSE_POSX];
    old_mouseposy = mouseaxis[0][MOUSE_POSY];
}
