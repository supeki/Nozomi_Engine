// JADEFRACTURE
// game_sound.c

#include "i_sound.h"

#include "game_defs.h"
#include "game_sound.h"

music_t music;
bool use_midi = false;

void S_Init(void)
{
	memset(&music, 0, sizeof(music_t));
	music.id = mus_none;
}

void S_PlayMusic(int id, bool loop)
{
	//if (id == music.id && I_MusicPlaying())
	//	return;
	
	if (id == mus_none) {
		I_StopMusic();
		music.id = mus_none;
		return;
	}

	I_PlayMusic(id, loop);
	music.id = id;
}