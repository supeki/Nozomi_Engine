// Nozomi Engine
// DOS/DJGPP backend
// dos_event.c

#include "dos_main.h"

#include "../../i_event.h"
#include "../../game_input.h"

int mouse_offx, mouse_offy;
uint16_t old_mouseposx = 0, old_mouseposy = 0;

void I_PollEvents(void)
{
    int j, c;
    poll_keyboard();
    poll_mouse();

    for (j = 0; j < MAX_PLAYERS; j++)
        for (c = CON_UP; c < NUMCONTROLS; c++)
            if (key[gamecontrolbinds[j][c][0]]) // holy fucking shit
                gamecontrols[j][c]++;
            else
                gamecontrols[j][c] = 0;

    mouseaxis[0][MOUSE_POSX] = (mouse_pos >> 16) - mouse_offx;
    mouseaxis[0][MOUSE_POSY] = (mouse_pos & 0x0000FFFF) - mouse_offy;
    mouseaxis[0][MOUSE_INPUTX] = mouseaxis[0][MOUSE_POSX] - old_mouseposx;
    mouseaxis[0][MOUSE_INPUTY] = mouseaxis[0][MOUSE_POSY] - old_mouseposy;
    old_mouseposx = mouseaxis[0][MOUSE_POSX];
    old_mouseposy = mouseaxis[0][MOUSE_POSY];

    if (mouse_b & 1)
        mousecontrols[0][MOUSE_LBUTTON]++;
    else
        mousecontrols[0][MOUSE_LBUTTON] = 0;
}
