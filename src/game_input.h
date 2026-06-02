// Nozomi Engine
// game_input.h

#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "game_defs.h"

typedef enum
{
	CON_UP,
	CON_DOWN,
	CON_LEFT,
	CON_RIGHT,
	CON_A, // Interact
	CON_B, // Sword / Item
	CON_C, // Jump
	CON_START, // Inventory
	// Menu Controls
	CON_CONFIRM,
	CON_BACK,
	// Extra Controls
	CON_X, // Swap Left 
	CON_Y, // Swap Right
	CON_Z, // Swap Sword/Item
	CON_SELECT, // Map (Maybe)
	NUMCONTROLS
} controls_e;

extern uint32_t gamecontrols[NUMCONTROLS];
extern uint32_t gamecontrolbinds[NUMCONTROLS][2];

bool G_ControlDown(uint8_t control, bool pressed);
void G_DefaultControls(void);

#endif
