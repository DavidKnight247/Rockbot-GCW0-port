#ifndef SCENESLIB_H
#define SCENESLIB_H

#include "defines.h"
#include "graphicslib.h"
#include "SDL/SDL_thread.h"

class scenesLib
{
public:
    scenesLib();
	void preloadScenes();
	void intro();
	void main_screen();
	struct st_position pick_stage();
	void th_draw_text(std::string text, short line);

private:
	// preload/umload
	void preload_intro();
	void preload_stage_select();
	void preload_faces();/// @TODO: move to graphLib and create a function show_face(st_position)
	void unload_faces();/// @TODO: move to graphLib and create a function show_face(st_position)
	void unload_intro();
	void upload_stage_select();
	void intro_presents();
	int option_picker_old(st_position, int);
	void mainMenu();
	void showCredits();
	unsigned short int select_player();
	void draw_lights_select_player(graphicsLib_gSurface& lights, int mode, int adjustX, int adjustY);


private:
	graphicsLib_gSurface INTRO_SURFACES[INTRO_SURFACES_COUNT];
	graphicsLib_gSurface STAGE_SELECT_SURFACES[STAGE_SELECT_COUNT];
	st_position selected_stage;

	SDL_Thread *_draw_text_thread;
};

#endif // SCENESLIB_H
