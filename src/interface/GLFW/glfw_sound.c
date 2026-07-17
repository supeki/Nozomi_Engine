// Nozomi Engine
// "SDL2" backend
// glfw_sound.c

// thats right, we're using sdl for sound (temporarily)

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "../../i_sound.h"
#include "../../i_system.h"
#include "../../game_defs.h"
#include "../../game_sound.h"

Mix_Music *current_song;

const char *jadefrac_to_sdl[NUMMUSIC] = {
	"\0",
	"mystic.s3m",
	"sirens.mod"
};

void I_StartupSound(void)
{
	Mix_Init(MIX_INIT_MID|MIX_INIT_OGG);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 2048);
}

void I_PlaySound(void) {}

void I_PlayMusic(int id, bool loop)
{
	if (music.id != mus_none)
		Mix_FreeMusic(current_song);
	
	if (use_midi)
		current_song = Mix_LoadMUS(va("data/audio/%s.mid", jadefrac_to_sdl[id]));
	else
		current_song = Mix_LoadMUS(va("data/audio/%s.mp3", jadefrac_to_sdl[id]));
	
	if (current_song == NULL) {
		I_printf("Couldn't play song: %s", jadefrac_to_sdl[id]);
		return;
	}
	
	if (loop)
		Mix_PlayMusic(current_song, -1);
	else
		Mix_PlayMusic(current_song, 0);
	
	Mix_VolumeMusic(music_volume);
}
void I_StopMusic(void)
{
	Mix_HaltMusic();
}

bool I_MusicPlaying(void)
{
	return (Mix_PlayingMusic() > 0);
}

void I_SetMusicVolume(int8_t value)
{
	Mix_VolumeMusic(value);
}
