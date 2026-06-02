// Nozomi Engine
// NDS backend
// nds_sound.c

#include <maxmod9.h>

#include "nds_main.h"
#include "soundbank.h"

#include "../../i_sound.h"
#include "../../game_defs.h"
#include "../../game_sound.h"

int jadefrac_to_maxmod[NUMMUSIC] = {
	-1,
	MOD_PRESENTATION
};

void I_StartupSound(void)
{
	mmInitDefault("nitro:/soundbank.bin");
}

void I_PlaySound(void)
{
}

void I_PlayMusic(int id, bool loop)
{
	bool load = true;
	id = jadefrac_to_maxmod[id];
	
	if (!mmLoad(id)) {
		mmUnload(jadefrac_to_maxmod[music.id]);
		if (!mmLoad(id)) {
			printf("Unable to load music ID %d!\n", id);
			load = false;
		}
	}
	
	if (!load)
		return;
	
	if (loop)
		mmStart(MOD_PRESENTATION, MM_PLAY_LOOP);
	else
		mmStart(MOD_PRESENTATION, MM_PLAY_ONCE);
}

void I_StopMusic(void)
{
	mmStop();
}

bool I_MusicPlaying(void)
{
	return mmActive();
}