// JADEFRACTURE
// game_sound.h

#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "game_defs.h"

typedef enum
{
	mus_none,
	mus_test,
	NUMMUSIC
} music_e;

typedef struct
{
	int id;
} music_t;

extern music_t music;
extern bool use_midi;

void S_PlayMusic(int id, bool loop);

#endif