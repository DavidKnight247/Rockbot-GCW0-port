#ifndef SOUND_H
#define SOUND_H

#include <stdio.h>
#include <SDL/SDL_mixer.h>

void play_sfx(int sfx) {
	if (game_config.sound_enabled != 1) {
		return;
	}

	if (sfx_list[sfx] != NULL) {
		Mix_PlayChannel(-1, sfx_list[sfx], 0);
	}
}


void play_timed_sfx(int sfx, int time) {
	if (game_config.sound_enabled != 1) {
		return;
	}

	if (sfx_list[sfx] != NULL) {
		Mix_PlayChannelTimed(-1, sfx_list[sfx], -1 , time);
	}
}

// must follow the oders defined in enum SFX_LIST (types.h)
void load_all_sfx() {
	char buffer[255];
	int i = 0;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "npc_hit.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "npc_killed.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "player_hit.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "player_shot.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "player_jump.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;


	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "cursor.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "stage_selected.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "got_energy_pill.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "got_item.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "shot_reflected.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "door_open.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "got_weapon.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "teleport.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "implosion.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "player_death.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "destrin_ship.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "train_chunk.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, "skull_castle_intro.wav");
	sfx_list[i] = Mix_LoadWAV(buffer);
	i++;


	//Mix_Volume(-1, MIX_MAX_VOLUME);
}


void load_sfx(char sfx_file[255], Mix_Chunk *sound) {
	char buffer[255];
	SDL_RWops *fileRW;

	sprintf(buffer, "%sdata/sfx/%s", FILEPATH, sfx_file);
	//printf(">>sound.h - loading '%s' sfx file...", buffer);
	fileRW = SDL_RWFromFile(buffer, "rb");
	if (!fileRW) {
		fprintf(stderr, "\n<< ERROR: Could not open '%s' file.\n", buffer);
	}
	sound = Mix_LoadWAV_RW(fileRW, 1);
	if (sound == NULL) {
		fprintf(stderr, "\n<< ERROR: Unable to load WAV. Error: %s\n", Mix_GetError());
	} else {
		printf("done!\n");
	}
}



void init_audio() {
        if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 1024) < 0) {
		printf("Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}
}

void load_music(char *music_file) {
	char buffer[255];

	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
	sprintf(buffer, "%sdata/music/%s", FILEPATH, music_file);
	printf(">> load_music - filename: '%s' <<\n", buffer);
	music = Mix_LoadMUS(buffer);
	if (!music) {
		printf("Mix_LoadMUS(\"%s\"): %s\n", music_file, Mix_GetError());
		exit(-1);
	}
}

void load_boss_music(char *music_file) {
	char buffer[255];

	if (boss_music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(boss_music);
		music = NULL;
	}
	sprintf(buffer, "%sdata/music/%s", FILEPATH, music_file);
	boss_music = Mix_LoadMUS(buffer);
	if (!boss_music) {
		printf("ERROR - load_boss_music - Mix_LoadMUS(\"%s\"): %s\n", music_file, Mix_GetError());
		exit(-1);
	} else {
		printf(">> load_boss_music - loaded: %p <<\n", boss_music);
	}
}



void play_music() {
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

void play_boss_music() {
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

void load_stage_music (int stage_n) {
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


void stop_music() {
	if (music != NULL) {
		Mix_HaltMusic();
		Mix_FreeMusic(music);
		music = NULL;
	}
}

void close_audio() {
	Mix_FreeMusic(music);
	Mix_CloseAudio();
}

void sound_loop() {}

//#endif

#endif // SOUND_H


