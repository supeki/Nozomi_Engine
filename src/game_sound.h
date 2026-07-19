// Nozomi Engine
// game_sound.h

#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "game_defs.h"

typedef enum
{
	mus_none,
	mus_test,
	mus_more,
	mus_evenmore,
	NUMMUSIC
} music_e;

typedef struct
{
	int id;
} music_t;

extern music_t music;
extern bool use_midi;
extern int8_t sound_volume, music_volume;

void S_ChangeSoundVolume(int8_t value);
void S_SetSoundVolume(int8_t value);

void S_PlayMusic(int id, bool loop);
void S_ChangeMusicVolume(int8_t value);
void S_SetMusicVolume(int8_t value);

#endif