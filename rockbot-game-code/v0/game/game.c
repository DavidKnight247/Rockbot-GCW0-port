/***************************************************************************
                          main.c  -  description
                             -------------------
    begin                : Seg Set  1 20:03:07 BRT 2003
    copyright            : (C) 2003 by Iuri Fiedoruk
	email                : protoman@upperland.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/*
todo:
- colocar opções/etc em arquivos .cfg
- fazer verificador de existência de arquivos
- fazer editor de telas dentro do jogo
- separar as coisas em units
- scrolling de tela
*/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>				// Standard IO - Padrão ANSI-C
#include <stdlib.h>				// Standard Lib - Padrão ANSI-C
#include <string.h>				// Biblioteca string.h - para o uso da função stcmp.

#include <SDL/SDL.h>				//Include da SDL
#include <SDL/SDL_image.h>		//Include da biblioteca SDL_Image
#include <SDL/SDL_mixer.h>	// Include da biblioteca SDL_Mixer
#include <SDL/SDL_ttf.h>		// Include da biblioteca SDL_ttf
#include <SDL/SDL_endian.h>
#include <SDL/SDL_mixer.h>



char FILEPATH[512];


#ifdef PLAYSTATION
	#include "ps2_modules.h"
	struct struct_sample {
		char *sample;
		int sampleSize;
	};
	struct struct_sample *sfx_player_hit, *sfx_player_jump, *sfx_player_shot, *sfx_npc_hit, *sfx_npc_killed, *sfx_ui_cursor, *BGMUSIC=NULL;
#endif




// contador para temporização de dialogo que evita repetição do teclado
int dialog_wait=0;
// indica se dialogo está ativo e qual o dialogo que está ativo
int dialog_active = 0;
SDL_Event event;

SDL_Joystick *joystick1, *joystick2;
TTF_Font *font=NULL;
int *keyState[2];

Mix_Music *music = NULL;
Mix_Music *boss_music = NULL;
char *sample;
int sample_size;


#if !defined(PLAYSTATION) && !defined(DINGUX)
	#define SCALE 2
#endif

SDL_Surface *game_screen;
//#if defined (SCALE) || defined (DINGUX)
#if defined (SCALE)
	SDL_Surface *game_screen_scaled;
#endif

#define QUICK_LOAD


// includes que precisam das variáveis declaradas acima
#include "format/file_structs.h"
#include "include/types.h"
#include "include/sound.h"
#include "include/preload.h"
#include "common.h"			// funções comuns ao jogo e ao editor
#include "projectiles.h"
#include "graphics.h"		// funções comuns ao jogo e ao editor
#include "object.h"
#include "game.h"			// funções do jogo
#include "include/config.h"
#include "intro.h"
#include "include/transfer.h"
#include "../common/dialogs.h"


int k;

#ifdef WIN32
	#undef main
#endif

#ifndef XBOX
int main(int argc, char *argv[]) {
#else
void XBoxStartup(int argc, char *argv[]) {
#endif

	SDL_RWops *fileRW;

	game_config.sound_enabled = 1;


	sprintf(FILEPATH, "%s", "");

	#ifdef PLAYSTATION
		SifIopReset(NULL, 0);

		/* SP193: Being creative (Do something while waiting for the slow IOP to be reset). =D */
		int main_id = GetThreadId();
		ChangeThreadPriority(main_id, 64);

#ifdef PS2DEBUG
		// USED FOR DEBUG - faster loading than using network for everything
		sprintf(FILEPATH, "%s", "mass:/Rockbot/");
#endif
		while(!SifIopSync()); /* Wait for the IOP to be reset. */

		/* Initialize and connect to all SIF services on the IOP. */
		SifInitRpc(0);
		SifInitIopHeap();
		SifLoadFileInit();
		fioInit();

		/* Apply the SBV LMB patch to allow modules to be loaded from a buffer in EE RAM. */
		sbv_patch_enable_lmb();

		/* PS2loadMinimal(); *//* SP193: Don't load rom0:SIO2MAN or be prepared to clash with SDL! */
		PS2loadUSB();

		if (!strncmp(argv[0], "cdfs", 4) || !strncmp(argv[0], "cdrom", 5)) {
			PS2loadCD();
			sprintf(FILEPATH, "%s", "cdfs:");
		}

		//printf("DEBUG - path: %s\n", argv[0]);
	#else
		#ifndef WIN32
			#ifdef DINGUX
				strncpy (FILEPATH, argv[0], strlen(argv[0])-11);
			#else
				strncpy (FILEPATH, argv[0], strlen(argv[0])-7);
				printf("Linux: set FILEPATH: %s\n", FILEPATH);
			#endif
		#endif
	#endif

#if defined (DINGUX)
        FILE* stdoutFP;
        char stdoutBuffer[512];
        sprintf(stdoutBuffer, "%sdata/stdout.txt", FILEPATH);
        stdoutFP = freopen(stdoutBuffer, "w", stdout);
        if (!stdoutFP) {
                printf("ERROR: Cannout redirect stdout to file '%s'.\n", stdoutBuffer);
        }
#endif




	keyState[0] = (int *)malloc(KEYNUMBER * sizeof(int));
	keyState[1] = (int *)malloc(KEYNUMBER * sizeof(int));
	char buffer [255];
	if ( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_JOYSTICK|SDL_INIT_TIMER|SDL_INIT_AUDIO) < 0 ) {
		printf("SDL Error\n");
		printf("Unable to init SDL: %s\n", SDL_GetError());
	}

	TTF_Init();
	#ifdef WIN32
		sprintf(buffer, "%sdata/fonts/kongtext.ttf", FILEPATH);
	#else
		sprintf(buffer, "%sdata/fonts/kongtext.ttf", FILEPATH);
	#endif
	fileRW = SDL_RWFromFile(buffer, "rb");
	if (!fileRW) {
		printf("Error opening '%s' font\n", buffer);
		return 0;
	}
	font = TTF_OpenFontRW(fileRW, 1, 8);





	RES_DIFF_W = (MAX_W-RES_W)*0.5;
	RES_DIFF_H = (MAX_H-RES_H)*0.5;

	printf("LOG: Will start screen...");

	#ifdef PLAYSTATION
		game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
	#elif defined(DINGUX)
		//game_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, RES_W, RES_H, 8, 0, 0, 0, 255);
		//game_screen_scaled = SDL_SetVideoMode(320, 240, 8, SDL_SWSURFACE);
		game_screen = SDL_SetVideoMode(320, 200, 8, SDL_SWSURFACE);
	#else
		#ifndef SCALE
			game_screen = SDL_SetVideoMode(RES_W, RES_H, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
		#else
			game_screen = SDL_CreateRGBSurface(SDL_SWSURFACE, RES_W, RES_H, 8, 0, 0, 0, 255);
			game_screen_scaled = SDL_SetVideoMode(RES_W*2, RES_H*2, 8, SDL_HWSURFACE|SDL_DOUBLEBUF);
			SDL_SetColors(game_screen_scaled, colormap, 0, COLOR_COUNT);
		#endif
		SDL_WM_SetCaption(PROJECT_NAME,"Icon Title");
	#endif
	SDL_SetColors(game_screen, colormap, 0, COLOR_COUNT);

	init_audio();

	printf("INFO: There are %d joysticks attached\n", SDL_NumJoysticks());
	SDL_JoystickEventState(SDL_ENABLE);
	joystick1 = SDL_JoystickOpen(0);
	if (joystick1) {
		printf("Opened Joystick 1\n");
		printf("Name: %s\n", SDL_JoystickName(0));
		printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joystick1));
		printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joystick1));
		printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joystick1));
	} else {
		printf("Couldn't open Joystick 1\n");
	}
	joystick2 = SDL_JoystickOpen(1);
	if (joystick2) {
		printf("Opened Joystick 2\n");
	} else {
		printf("Couldn't open Joystick 2\n");
	}

	atexit(SDL_Quit);
	SDL_ShowCursor( SDL_DISABLE );



	init_main_vars();



	load_all_sfx();

	backup_colormap();

	preload_main();

	struct pos picked_stage;


#ifndef QUICK_LOAD
	picked_stage = intro_screen();
#else
	picked_stage.y = 0;
	picked_stage.x = 2;
	game_config.selected_player = 2;
	FILE* fp;
	char tempBuffer[150];
	sprintf(tempBuffer, "%sdata/001.sav", FILEPATH);
	fp = fopen(tempBuffer, "rb");
	fread (&save_001, sizeof(struct st_save), 1, fp);
	fclose(fp);
#endif
	STAGE_N = picked_stage.y*3 + picked_stage.x + 1;
	// 0.0: apebot, 0.1: daisiebot, 0.2: seahorsebot
	// 1.0: mummy, 1.1: NONE (skull castle), 1.2: magebot
	// 2.0: dynamitebot, 2.1: spikebot, 2.2: technobot
	checkpoint.map = 0;

	load_stage_music (STAGE_N);




	if (game_init(picked_stage) == 0) {
		sprintf(buffer, "Game_init() failed\n");
		show_error(CRITICAL, buffer);
		exit(-1);
	}

	updateScreen(game_screen);

	play_music();


	restore_colormap();
	exec_colorcycle(game_screen);

	init_dialogs();
	exec_colorcycle(game_screen);

	drawMap3rdLevel(game_screen);

	exec_colorcycle(game_screen);
	showReadyAnimation();

	char fps_string[32];
	int fps_timer = getTimer()+1000;
	int fps_counter = 0;
	sprintf(fps_string, "%d", fps_counter);
	while (fim == 0) {
		update_sprites(1);
		sound_loop();
		exec_colorcycle(game_screen);

		#ifdef PLAYSTATION
			RotateThreadReadyQueue(_MIXER_THREAD_PRIORITY);
		#endif

		draw_text(290, 10, fps_string, game_screen);
		updateScreen(game_screen);
		fps_counter++;
		if (fps_timer <= getTimer()) {
			sprintf(fps_string, "%d", fps_counter);
			fps_counter = 0;
			fps_timer = getTimer()+1000;
		}
		// fps limiter
		if (fps_counter > 130) {
			waitTime(5);
		}
	}

	leave_game();
	return(0);
}
