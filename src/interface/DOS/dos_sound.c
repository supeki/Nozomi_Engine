// Nozomi Engine
// DOS/DJGPP backend
// dos_sound.c

#include "dos_main.h"

#include "../../i_sound.h"
#include "../../i_system.h"
#include "../../game_defs.h"
#include "../../game_sound.h"

MIDI *current_song;

const char *jadefrac_to_dos[NUMMUSIC] = {
	"\0",
	"eshop_8.mid"
};

void I_StartupSound(void)
{
	install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
}

void I_ShutdownSound(void)
{
	remove_sound();
}

void I_PlaySound(void)
{
}

void I_PlayMusic(int id, bool loop)
{
	if (music.id != mus_none)
		destroy_midi(current_song);
	
	current_song = load_midi(va("data/audio/%s", jadefrac_to_dos[id]));
	
	if (current_song == NULL) {
		I_printf("Couldn't play song: %s", jadefrac_to_dos[id]);
		return;
	}
	
	if (loop)
		play_midi(current_song, 1);
	else
		play_midi(current_song, 0);
	
	set_volume(-1, music_volume);
}

void I_SetSoundVolume(int8_t value)
{
	set_volume(value, -1);
}

void I_StopMusic(void)
{
	stop_midi();
}

bool I_MusicPlaying(void)
{
	return (midi_pos > -1);
}

void I_SetMusicVolume(int8_t value)
{
	set_volume(-1, value);
}