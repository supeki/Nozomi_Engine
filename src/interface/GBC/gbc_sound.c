// Nozomi Engine
// GBC backend
// gbc_sound.c

#include "gbc_main.h"

#include "../../i_sound.h"
#include "../../i_system.h"
#include "../../game_defs.h"
#include "../../game_sound.h"

int jadefrac_to_gbc[NUMMUSIC] = {
	-1,
	-1
};

void I_StartupSound(void)
{
}

void I_PlaySound(void)
{
}

void I_PlayMusic(int id, bool loop)
{
	//bool load = true;
	//id = jadefrac_to_gbc[id];
	
	//if (!mmLoad(id)) {
	//	mmUnload(jadefrac_to_gbc[music.id]);
	//	if (!mmLoad(id)) {
	//		I_printf("Unable to load music ID %d!\n", id);
	//		load = false;
	//	}
	//}
	
	//if (!load)
	//	return;
	
	//if (loop)
	//	mmStart(id, MM_PLAY_LOOP);
	//else
	//	mmStart(id, MM_PLAY_ONCE);
}

void I_StopMusic(void)
{
	//mmStop();
}

bool I_MusicPlaying(void)
{
	return false;
}

void I_SetMusicVolume(int8_t value)
{
	(void)value;
}

void I_SetSoundVolume(int8_t value)
{
	(void)value;
}