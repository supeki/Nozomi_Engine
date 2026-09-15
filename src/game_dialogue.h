// Nozomi Engine
// game_dialogue.h

#ifndef GAME_DIALOGUE_H
#define GAME_DIALOGUE_H

#include "game_defs.h"

#define DEFAULT_CHARTIME 3

void D_StartDialogue(uint32_t dialogue_num);
void D_UpdateDialogue(void);
void D_DrawDialogue(void);

extern bool in_diag;
extern uint32_t curdiag;
extern uint32_t cur_char;
extern uint32_t cur_char_timer;
extern uint32_t time_per_char;

#endif
