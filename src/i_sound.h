// Nozomi Engine
// i_sound.h

#ifndef I_SOUND_H
#define I_SOUND_H

#include "game_defs.h"

void I_StartupSound(void);

void I_PlaySound(void);

void I_PlayMusic(int id, bool loop);
void I_StopMusic(void);
bool I_MusicPlaying(void);
void I_SetMusicVolume(int8_t value);

#endif
