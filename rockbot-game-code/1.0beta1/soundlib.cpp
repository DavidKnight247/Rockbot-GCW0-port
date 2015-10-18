#include "soundlib.h"

extern string FILEPATH;

soundLib::soundLib()
{
	music = NULL;
	boss_music = NULL;
	sound_enabled = true;
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
	load_all_sfx();
}

soundLib::~soundLib()
{
	close_audio();
}


void soundLib::play_sfx(int sfx) {
	if (!sound_enabled) {
		return;
	}

	if (sfx_list[sfx] != NULL) {
		Mix_PlayChannel(-1, sfx_list[sfx], 0);
	} else {
		cout << "Error: soundLib::play_sfx - null sfx\n";
	}
}

void soundLib::play_repeated_sfx(int sfx, int loops) {
    if (!sound_enabled) {
        return;
    }

    if (sfx_list[sfx] != NULL) {
        Mix_PlayChannel(-1, sfx_list[sfx], loops);
    } else {
        cout << "Error: soundLib::play_sfx - null sfx\n";
    }
}


void soundLib::play_timed_sfx(int sfx, int time) {
	if (!sound_enabled) {
		return;
	}

	if (sfx_list[sfx] != NULL) {
		Mix_PlayChannelTimed(-1, sfx_list[sfx], -1 , time);
	}
}

// must follow the oders defined in enum SFX_LIST (types.h)
void soundLib::load_all_sfx() {
	int i = 0;
	string filename;

	filename = FILEPATH + "data/sfx/npc_hit.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/npc_killed.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/player_hit.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/player_shot.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/player_jump.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/cursor.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/stage_selected.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/got_energy_pill.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/got_item.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/shot_reflected.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/door_open.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/got_weapon.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/teleport.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/implosion.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/player_death.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/destrin_ship.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/train_chunk.wav";
	sfx_list[i] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/skull_castle_intro.wav";
	sfx_list[SFX_SKULL_CASTLE_INTRO] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/charged_shot.wav";
	sfx_list[SFX_PLAYER_CHARGED_SHOT] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/charging1.wav";
	sfx_list[SFX_CHARGING1] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/charging2.wav";
	sfx_list[SFX_CHARGING2] = Mix_LoadWAV(filename.c_str());
	i++;

	filename = FILEPATH + "data/sfx/big_explosion.wav";
	sfx_list[SFX_BIG_EXPLOSION] = Mix_LoadWAV(filename.c_str());
	i++;


	//Mix_Volume(-1, MIX_MAX_VOLUME);
}


void soundLib::load_sfx(string sfx_file, Mix_Chunk *sound) {
	string filename;
	SDL_RWops *fileRW;

	filename = FILEPATH + "data/sfx/" + sfx_file;
	fileRW = SDL_RWFromFile(filename.c_str(), "rb");
	if (!fileRW) {
		cout << "Error: Could not load file '" << filename << "'\n";
	}
	sound = Mix_LoadWAV_RW(fileRW, 1);
	if (sound == NULL) {
		cout << "Error: Unable to load WAV: '" << Mix_GetError() << "'\n";
	}
}




void soundLib::load_music(string music_file) {
	string filename;

	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	filename = FILEPATH + "data/music/" + music_file;
	//std::cout << "soundLib::load_music - filename: " << filename << std::endl;
	music = Mix_LoadMUS(filename.c_str());
	if (!music) {
		cout << "Error in soundLib::load_music::Mix_LoadMUS('" << filename << "': '" << Mix_GetError() << "'\n";
		exit(-1);
	}
}

void soundLib::load_boss_music(string music_file) {
	string filename;

	if (boss_music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(boss_music);
		music = NULL;
	}
	filename = FILEPATH + "data/music/" + music_file;
	//std::cout << "soundLib::load_boss_music - filename: " << filename << std::endl;
	boss_music = Mix_LoadMUS(filename.c_str());
	if (!boss_music) {
		cout << "Error in soundLib::load_boss_music::Mix_LoadMUS('" << filename << "': '" << Mix_GetError() << "'\n";
		exit(-1);
	}
}



void soundLib::play_music() {
	// toca a música
	if (music) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (Mix_PlayMusic(music, -1) == -1) {
			printf("<<<<<<<<<<<<< Mix_PlayMusic: %s\n", Mix_GetError());
			exit(-1);
		}
	} else {
		printf(">> play_music ERROR: music is null\n");
	}
}

void soundLib::play_boss_music() {
	// toca a música
	if (boss_music) {
		Mix_VolumeMusic(MIX_MAX_VOLUME);
		if (Mix_PlayMusic(boss_music, -1) == -1) {
			printf("<<<<<<<<<<<<< Mix_PlayMusic: %s\n", Mix_GetError());
			exit(-1);
		}
	} else {
		printf(">> play_boss_music ERROR: boss_music is null\n");
	}
}

void soundLib::load_stage_music (int stage_n) {
	if (stage_n == SKULLCASTLE1) {
		load_music("castle.mod");
	} else if (stage_n == SPIKEBOT)  {
		load_music("spike.mod");
	} else if (stage_n == SEAHORSEBOT)  {
		load_music("seahorse.mod");
	} else if (stage_n == DAISIEBOT)  {
		load_music("daisie.mod");
	} else if (stage_n == DYNAMITEBOT)  {
		printf(">> load_music() dynamite.mod<<\n");
		load_music("dynamite.mod");
	} else if (stage_n == TECHNOBOT)  {
		load_music("techno.mod");
	} else if (stage_n == MAGEBOT)  {
		load_music("mage.mod");
	} else if (stage_n == MUMMYBOT)  {
		load_music("mummy.mod");
	} else if (stage_n == APEBOT)  {
		load_music("ape.mod");
	} else {
		load_music("bgmusic.mod");
	}
}


void soundLib::stop_music() {
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
}

void soundLib::close_audio() {
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void sound_loop() {}

