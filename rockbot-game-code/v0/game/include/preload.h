#ifndef PRELOAD_H
#define PRELOAD_H

#include <SDL/SDL.h>

SDL_Surface *loadSpriteFile(const char *buffer);
SDL_Surface *surfaceFromFile(char filename[255]);
void loadGameNpcs (int n);
void load_map_links(void);
void load_map_links(void);

// used for preloading
enum FACES_ENUM {
	FACES_KANOTUS,
	FACES_CANDY,
	FACES_BETABOT,
	FACES_ROCKBOT,
	FACES_APEBOT,
	FACES_DAISIEBOT,
	FACES_SEAHORSEBOT,
	FACES_MUMMYBOT,
	FACES_MAGEBOT,
	FACES_DYNAMITEBOT,
	FACES_SPIKEBOT,
	FACES_TECHNOBOT,
	FACES_DR_DESTRIN,
	FACES_BETABOT_NOEYES,
	FACES_ROCKBOT_NOEYES,
	FACES_BETABOT_EYES,
	FACES_ROCKBOT_EYES,
	FACES_COUNT
};




enum INTRO_SURFACES_ENUM {
	INTRO_SURFACES_UPPERLAND_PRESENTS, //presents.png
	INTRO_SURFACES_CITY_BG, //scenes/city_intro1.png
	INTRO_SURFACES_KANOTUS, //sprites/canotus.png
	INTRO_SURFACES_ROCKBOT, //sprites/sprites_p1.png
	INTRO_SURFACES_BETABOT, //sprites/sprites_p2.png
	INTRO_SURFACES_LAB_BG, //scenes/lab_intro1.png
	INTRO_SURFACES_INTRO_SCREEN, //logo.png
	INTRO_SURFACES_PLAYER_SELECT_BG, //backgrounds/player_select.png
	INTRO_SURFACES_PLAYER_SELECT_LIGHTS, //backgrounds/lights.png
	INTRO_SURFACES_COUNT
};

enum STAGE_SELECT_ENUM {
	INTRO_SURFACES_STAGE_SELECT_BG,
	STAGE_SELECT_COUNT
};

SDL_Surface *INTRO_SURFACES[INTRO_SURFACES_COUNT];
SDL_Surface *FACES_SURFACES[FACES_COUNT];
SDL_Surface *STAGE_SELECT_SURFACES[STAGE_SELECT_COUNT];




void face_file(int face_n, char face_str[255]) {
	if (face_n == FACES_KANOTUS) {
		sprintf(face_str, "%s", "canotus_face.png");
	} else if (face_n == FACES_CANDY) {
		sprintf(face_str, "%s", "candybot.png");
	} else if (face_n == FACES_BETABOT) {
		sprintf(face_str, "%s", "betabot.png");
	} else if (face_n == FACES_ROCKBOT) {
		sprintf(face_str, "%s", "rockbot.png");
	} else if (face_n == FACES_APEBOT) {
		sprintf(face_str, "%s", "apebot.png");
	} else if (face_n == FACES_DAISIEBOT) {
		sprintf(face_str, "%s", "daisiebot.png");
	} else if (face_n == FACES_SEAHORSEBOT) {
		sprintf(face_str, "%s", "seahorsebot.png");
	} else if (face_n == FACES_MUMMYBOT) {
		sprintf(face_str, "%s", "mummybot.png");
	} else if (face_n == FACES_MAGEBOT) {
		sprintf(face_str, "%s", "magebot.png");
	} else if (face_n == FACES_DYNAMITEBOT) {
		sprintf(face_str, "%s", "dynamitebot.png");
	} else if (face_n == FACES_SPIKEBOT) {
		sprintf(face_str, "%s", "spikebot.png");
	} else if (face_n == FACES_TECHNOBOT) {
		sprintf(face_str, "%s", "technobot.png");
	} else if (face_n == FACES_DR_DESTRIN) {
		sprintf(face_str, "%s", "dr_destrin.png");
	} else if (face_n == FACES_BETABOT_NOEYES) {
		sprintf(face_str, "%s", "betabot_no_eyes.png");
	} else if (face_n == FACES_ROCKBOT_NOEYES) {
		sprintf(face_str, "%s", "rockbot_no_eyes.png");
	} else if (face_n == FACES_BETABOT_EYES) {
		sprintf(face_str, "%s", "betabot_eyes.png");
	} else if (face_n == FACES_ROCKBOT_EYES) {
		sprintf(face_str, "%s", "rockbot_eyes.png");
	}
}

void preload_intro() {

	char tempBuffer[255];

	sprintf(tempBuffer, "%sdata/images/presents.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/scenes/city_intro1.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_CITY_BG] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_CITY_BG], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/sprites/canotus.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_KANOTUS] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_KANOTUS], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/sprites/sprites_p1.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_ROCKBOT] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_ROCKBOT], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/sprites/sprites_p2.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_BETABOT] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_BETABOT], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/scenes/lab_intro1.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_LAB_BG] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_LAB_BG], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/logo.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/backgrounds/player_select.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG], colormap, 0, COLOR_COUNT);

	sprintf(tempBuffer, "%sdata/images/backgrounds/lights.png", FILEPATH);
	INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_LIGHTS] = surfaceFromFile(tempBuffer);
	SDL_SetColors(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_LIGHTS], colormap, 0, COLOR_COUNT);

	load_music("opening.mod");

}

void preload_stage_select() {
	//backgrounds/stage_select.png
	//backgrounds/stage_select_darkned.png
	//backgrounds/stage_select_highlighted.png
	//backgrounds/stage_select_highlighted_yellow.png
	//backgrounds/stage_select_item.png
	//backgrounds/stage_boss_intro.png
}





void preload_faces() {
	char buffer_file[255];
	char buffer_face[255];
	int i;

	for (i=0; i<FACES_COUNT; i++) {
		FACES_SURFACES[i] = NULL;
	}
	sprintf(buffer_face, "%s", "\0");

	for (i=0; i<FACES_COUNT; i++) {
		face_file(i, buffer_face);
		sprintf(buffer_file, "%sdata/images/faces/%s", FILEPATH, buffer_face);
		FACES_SURFACES[i] = surfaceFromFile(buffer_file);
		SDL_SetColors(FACES_SURFACES[i], colormap, 0, COLOR_COUNT);
	}
}


void unload_faces() {
	int i;

	for (i=0; i<FACES_COUNT; i++) {
		if (FACES_SURFACES[i]) {
			SDL_FreeSurface(FACES_SURFACES[i]);
		}
	}
}

void unload_intro() {
	int i;

	for (i=0; i<INTRO_SURFACES_COUNT; i++) {
		if (INTRO_SURFACES[i]) {
			SDL_FreeSurface(INTRO_SURFACES[i]);
		}
	}
}

void upload_stage_select() {
	int i;

	for (i=0; i<STAGE_SELECT_COUNT; i++) {
		if (STAGE_SELECT_SURFACES[i]) {
			SDL_FreeSurface(STAGE_SELECT_SURFACES[i]);
		}
	}
}


void preload_main(void) {
	preload_stage_select();
	preload_faces();
	loadGameNpcs (1);
	load_map_links();
	load_boss_music("boss_battle.mod");
}

#endif // PRELOAD_H
