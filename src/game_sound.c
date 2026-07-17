// Nozomi Engine
// game_sound.c

#include "i_sound.h"

#include "game_defs.h"
#include "game_sound.h"

music_t music;
bool use_midi = false;

int8_t sound_volume = 64, music_volume = 64;

void S_Init(void)
{
	memset(&music, 0, sizeof(music_t));
	music.id = mus_none;
}

void S_ChangeSoundVolume(int8_t value)
{
	int16_t new_value;
	new_value = sound_volume + value;
	
	if (new_value < 0)
		new_value = 0;
	if (new_value > 127)
		new_value = 127;
	
	sound_volume = new_value;
	I_SetSoundVolume(sound_volume);
}

void S_SetSoundVolume(int8_t value)
{
	if (value < 0)
		value = 0;
	if (value > 127)
		value = 127;
	
	sound_volume = value;
	I_SetSoundVolume(sound_volume);
}

void S_PlayMusic(int id, bool loop)
{
	if (id == music.id && I_MusicPlaying())
		return;
	
	if (id == mus_none) {
		I_StopMusic();
		music.id = mus_none;
		return;
	}

	I_PlayMusic(id, loop);
	music.id = id;
}

void S_ChangeMusicVolume(int8_t value)
{
	int16_t new_value;
	new_value = music_volume + value;
	
	if (new_value < 0)
		new_value = 0;
	if (new_value > 127)
		new_value = 127;
	
	music_volume = new_value;
	I_SetMusicVolume(music_volume);
}

void S_SetMusicVolume(int8_t value)
{
	if (value < 0)
		value = 0;
	if (value > 127)
		value = 127;
	
	music_volume = value;
	I_SetMusicVolume(music_volume);
}