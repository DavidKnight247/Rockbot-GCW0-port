#ifndef SOUNDLIB_H
#define SOUNDLIB_H

#include <iostream>
using namespace std;

#include <SDL/SDL_mixer.h>
#include "defines.h"

class soundLib
{
public:
    soundLib();
	~soundLib();
	void play_sfx(int sfx);
    void play_repeated_sfx(int sfx, int loops);
	void play_timed_sfx(int sfx, int time);
	void load_all_sfx();
	void load_sfx(string sfx_file, Mix_Chunk *sound);
	void load_music(string music_file);
	void load_boss_music(string music_file);
	void play_music();
	void play_boss_music();
	void load_stage_music (int stage_n);
	void stop_music();
	void close_audio();
	void sound_loop();

private:


public:

private:
	bool sound_enabled;
	Mix_Chunk *sfx_list[SFX_COUNT];
	Mix_Music *music;
	Mix_Music *boss_music;
};

#endif // SOUNDLIB_H
