// Nozomi Engine
// WinCE backend
// ce_sound.c

#include "ce_main.h"

#include "../../i_sound.h"
#include "../../i_system.h"
#include "../../game_defs.h"
#include "../../game_sound.h"
#include "../../game_video.h"

const char *jadefrac_to_wince[NUMMUSIC] = {
	"\0",
	"tech_demo_boss.xm"
};

void I_StartupSound(void)
{
}

void I_PlaySound(void)
{
}

void I_PlayMusic(int id, bool loop)
{
}

void I_SetSoundVolume(int8_t value)
{
}

void I_StopMusic(void)
{
}

bool I_MusicPlaying(void)
{
	return false;
}

void I_SetMusicVolume(int8_t value)
{
}
