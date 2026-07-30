// Nozomi Engine
// game_dialogue.c

#include "game_defs.h"
#include "game_dialogue.h"
#include "game_font.h"
#include "game_gfx.h"
#include "game_input.h"
#include "game_main.h"
#include "game_object.h"
#include "game_player.h"
#include "game_sound.h"
#include "game_video.h"
#include "game_world.h"
#include "i_system.h"

// dialogue
// \c requires button to continue to next
// \d requires an option
// \e requires a button to end
static char *dialogue[] = {
	"So... there's a Man here.",
	"He smiles and offers you an Egg.^",
	"You take the Egg.\n[The Egg is in your Key Items.]#",
	"The Man shakes his head.\n[Somehow, you possess the Egg anyways.]#",
	"Well, there was a Man here.",
	"But now they're gone.#",
	"Just a tree.#"
};

static char options[][2][256] = {
	{"Accept", "Deny"}
};

bool in_diag = false;
uint32_t curdiag = -1;
uint32_t cur_char;
uint32_t cur_char_timer;
uint32_t time_per_char;
uint32_t cur_option;

void D_StartDialogue(uint32_t dialogue_num)
{
	in_diag = true;
	curdiag = dialogue_num;
	cur_char = 0;
	cur_char_timer = 0;
	time_per_char = DEFAULT_CHARTIME;
	cur_option = 0;
}

void D_UpdateDialogue(void)
{
	if (!in_diag)
		return;
	
	if (cur_char >= strlen(dialogue[curdiag])) {
		if (dialogue[curdiag][cur_char-1] != '#') {
			if (dialogue[curdiag][cur_char-1] == '^') {
				if (G_ControlDown(PLAYER_ONE, CON_UP, true))
					cur_option--;
				if (G_ControlDown(PLAYER_ONE, CON_DOWN, true))
					cur_option++;
				
				if (cur_option > 1)
					cur_option = 0;
				
				if (G_ControlDown(PLAYER_ONE, CON_A, true))
					D_StartDialogue(curdiag+cur_option+1);
			} else
				if (G_ControlDown(PLAYER_ONE, CON_A, true))
					D_StartDialogue(curdiag+1);
		} else if (cur_char_timer < time_per_char*8 && !G_ControlDown(PLAYER_ONE, CON_A, true))
			cur_char_timer++;
		else
			switch (dialogue[curdiag][cur_char]) {
				default:
					in_diag = false;
					break;
			}
	} else {
		if (G_ControlDown(PLAYER_ONE, CON_B, false))
			cur_char_timer+=time_per_char;
		else if (G_ControlDown(PLAYER_ONE, CON_C, false) && cur_char < strlen(dialogue[curdiag])) {
			cur_char_timer = 0;
			cur_char = strlen(dialogue[curdiag]);
		} else
			cur_char_timer++;
		
		if (cur_char_timer >= time_per_char || dialogue[curdiag][cur_char] == '\n') {
			cur_char++;
			cur_char_timer = 0;
		}
	}
}

void D_DrawDialogue(void)
{
	char *str = malloc(cur_char+1);
	memset(str, 0, cur_char+1);
	str[cur_char] = '\0'; 
	
	for (int i = 0; i < cur_char; i++)
		if (i < strlen(dialogue[curdiag]) && dialogue[curdiag][i] != '#' && dialogue[curdiag][i] != '^')
			str[i] = dialogue[curdiag][i];
		
	V_DrawCroppedBitmap(gfx_textbox, 0, VID_HEIGHT-32, 0, 0, 8, 8, 0);
	V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-32, 16, 0, 8, 8, 0);
	V_DrawCroppedBitmap(gfx_textbox, 0, VID_HEIGHT-8, 0, 16, 8, 8, 0);
	V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-8, 16, 16, 8, 8, 0);
	
	for (int i = 0; i < VID_WIDTH/8 - 2; i++) {
		V_DrawCroppedBitmap(gfx_textbox, 8 + i*8, VID_HEIGHT-32, 8, 0, 8, 8, 0);
		V_DrawCroppedBitmap(gfx_textbox, 8 + i*8, VID_HEIGHT-8, 8, 16, 8, 8, 0);
	}
	
	for (int i = 0; i < 2; i++) {
		V_DrawCroppedBitmap(gfx_textbox, 0, VID_HEIGHT-24 + i*8, 0, 8, 8, 8, 0);
		V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-24 + i*8, 16, 8, 8, 8, 0);
	}
	
	for (int y = 0; y < 2; y++)
		for (int x = 0; x < VID_WIDTH/8 - 2; x++)
			V_DrawCroppedBitmap(gfx_textbox, 8 + x*8, VID_HEIGHT-24 + y*8, 8, 8, 8, 8, 0);
	
	V_DrawText(str, 8, VID_HEIGHT-24, 0);
	//V_DrawText(va("%d/%d", cur_char, strlen(dialogue[curdiag])), 0, 8, 0);

	if (cur_char >= strlen(dialogue[curdiag]))
		if (dialogue[curdiag][cur_char-1] == '^') {
			V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*9), VID_HEIGHT-64, 0, 0, 8, 8, 0);
			V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-64, 16, 0, 8, 8, 0);
			V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*9), VID_HEIGHT-40, 0, 16, 8, 8, 0);
			V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-40, 16, 16, 8, 8, 0);
			
			for (int i = 0; i < 7; i++) {
				V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*8) + i*8, VID_HEIGHT-64, 8, 0, 8, 8, 0);
				V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*8) + i*8, VID_HEIGHT-40, 8, 16, 8, 8, 0);
			}
			
			for (int i = 0; i < 2; i++) {
				V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*9), VID_HEIGHT-56 + i*8, 0, 8, 8, 8, 0);
				V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-8, VID_HEIGHT-56 + i*8, 16, 8, 8, 8, 0);
			}
			
			for (int y = 0; y < 2; y++)
				for (int x = 0; x < 7; x++)
					V_DrawCroppedBitmap(gfx_textbox, VID_WIDTH-(8*8) + x*8, VID_HEIGHT-56 + y*8, 8, 8, 8, 8, 0);
			
			V_DrawText("Accept\nDeny", VID_WIDTH-(8*6), VID_HEIGHT-56, 0);
			V_DrawText(">", VID_WIDTH-(8*8), VID_HEIGHT-56+cur_option*8, 0);
		}
}