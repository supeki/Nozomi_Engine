// JADEFRACTURE
// SDL2 backend
// sdl_sound.c

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#include "sdl_main.h"

#include "../../i_sound.h"
#include "../../i_system.h"
#include "../../game_defs.h"
#include "../../game_sound.h"

Mix_Music *current_song;

const char *jadefrac_to_sdl[NUMMUSIC] = {
	"\0",
	"presentation"
};

void I_StartupSound(void)
{
	Mix_Init(MIX_INIT_MID|MIX_INIT_OGG);
	
	// Use Native MIDI on Windows :3
	#if !defined(WINDOWS)
	Mix_SetSoundFonts("data/soundfont.sf2");
	#endif
	
	// PSP doesn't support MIDI playback!?
	#if defined(PSP)
	use_midi = false;
	#endif
	
	Mix_OpenAudio(32000, MIX_DEFAULT_FORMAT, 2, 2048);
}

void I_PlaySound(void)
{
}

void I_PlayMusic(int id, bool loop)
{
	if (music.id != mus_none)
		Mix_FreeMusic(current_song);
	
	if (use_midi)
		current_song = Mix_LoadMUS(va("data/audio/%s.mid", jadefrac_to_sdl[id]));
	else
		current_song = Mix_LoadMUS(va("data/audio/%s.ogg", jadefrac_to_sdl[id]));
	
	if (current_song == NULL) {
		I_printf("Couldn't play song: %s", jadefrac_to_sdl[id]);
		return;
	}
	
	if (loop)
		Mix_PlayMusic(current_song, -1);
	else
		Mix_PlayMusic(current_song, 0);
}

void I_StopMusic(void)
{
	Mix_HaltMusic();
}

bool I_MusicPlaying(void)
{
	return (Mix_PlayingMusic() > 0);
}