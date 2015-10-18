#include "sceneslib.h"
#include "soundlib.h"

extern string FILEPATH;
extern soundLib soundManager;

#include "graphicslib.h"
extern graphicsLib graphLib;

#include "inputlib.h"
extern inputLib input;

#include "soundlib.h"
extern soundLib soundManager;

#include "stage_select.h"
#include "graphic/option_picker.h"
#include "file/file_io.h"

extern format_v1_0::st_game_config game_config;
extern format_v_2_0_1::st_save game_save;



// ********************************************************************************************** //
// ScenesLib handles all scinematic like intro and ending                                         //
// ********************************************************************************************** //
scenesLib::scenesLib()
{
    //preloadScenes();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::preloadScenes()
{
	//graphLib.draw_text(RES_W*0.5-45-graphLib.RES_DIFF_W, RES_H*0.5-15,"LOADING...");
	preload_stage_select();
	soundManager.load_boss_music("boss_battle.mod");
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::preload_intro() { /// @TODO: check is memory is being freed, otherwise, use local variables and load on-the-fly

	std::string filename;

	filename = FILEPATH + "data/images/presents.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]);

	filename = FILEPATH + "data/images/scenes/city_intro1.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_CITY_BG]);

	filename = FILEPATH + "data/images/sprites/canotus.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_KANOTUS]);

	filename = FILEPATH + "data/images/sprites/p1.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT]);

	filename = FILEPATH + "data/images/sprites/p2.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_BETABOT]);

	filename = FILEPATH + "data/images/scenes/lab_intro1.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_LAB_BG]);

	filename = FILEPATH + "data/images/logo.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN]);

	filename = FILEPATH + "data/images/backgrounds/player_select.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]);

	filename = FILEPATH + "data/images/backgrounds/lights.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_LIGHTS]);

	filename = FILEPATH + "data/images/backgrounds/capsules.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_CAPSULES]);

	filename = FILEPATH + "data/images/scenes/rockbot.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG]);

	filename = FILEPATH + "data/images/scenes/rockbot_half_sleep.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_HALF_SLEEP]);

	filename = FILEPATH + "data/images/scenes/rockbot_full_sleep.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_FULL_SLEEP]);

	filename = FILEPATH + "data/images/scenes/betabot.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_BETABOT_BIG]);

	filename = FILEPATH + "data/images/scenes/kanotus.png";
	graphLib.surfaceFromFile(filename, &INTRO_SURFACES[INTRO_SURFACES_KANOTUS_BIG]);


	soundManager.load_music("opening.mod");

}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::preload_stage_select() {
	//backgrounds/stage_select.png
	//backgrounds/stage_select_darkned.png
	//backgrounds/stage_select_highlighted.png
	//backgrounds/stage_select_highlighted_yellow.png
	//backgrounds/stage_select_item.png
	//backgrounds/stage_boss_intro.png
}









// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::unload_intro() {
	int i;

	for (i=0; i<INTRO_SURFACES_COUNT; i++) {
		if (INTRO_SURFACES[i].gSurface) {
			SDL_FreeSurface(INTRO_SURFACES[i].gSurface);
		}
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::upload_stage_select() {
	int i;

	for (i=0; i<STAGE_SELECT_COUNT; i++) {
		if (STAGE_SELECT_SURFACES[i].gSurface) {
			SDL_FreeSurface(STAGE_SELECT_SURFACES[i].gSurface);
		}
	}
}

void scenesLib::th_draw_text(std::string text, short int line)
{
	int line_pos_y = 120+line*15;
	graphLib.draw_progressive_text(10, line_pos_y, text, true, false, false);
	input.waitScapeTime(100);
}

int draw_threaded_text(void *data) {
	scenesLib* class_obj = reinterpret_cast<scenesLib*>(data);
	class_obj->th_draw_text("DECIDED TO PROTECT HIS OWN CITY,", 0);
	class_obj->th_draw_text("THE YOUNG PROF. KANOTUS CREATED", 1);
	class_obj->th_draw_text("A PROJECT IN THE CITY UNIVERSITY", 2);
	class_obj->th_draw_text("TO COMPOSE A POLICE TASK FORCE,", 3);
	class_obj->th_draw_text("MADE OF ELECTRONICS AND STEEL.", 4);
	return 0;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::intro()
{
	int cut=0;

	preload_intro();
	intro_presents();

	// do not cut here because we need to load the music
	graphLib.draw_progressive_text(RES_W*0.5-45-graphLib.RES_DIFF_W, RES_H*0.5-15, "20XX AD.", false, false);
	input.waitScapeTime(2000);


	soundManager.play_music();

	graphLib.set_colormap(-2);
	graphLib.blank_screen();
	graphLib.copyArea(st_rectangle(0, 0, INTRO_SURFACES[INTRO_SURFACES_CITY_BG].gSurface->w, INTRO_SURFACES[INTRO_SURFACES_CITY_BG].gSurface->h), st_position(10, 0), &INTRO_SURFACES[INTRO_SURFACES_CITY_BG], &graphLib.gameScreen);


	cut = graphLib.draw_progressive_text(10, 120,"THE YEAR 20XX IS FULL OF", true, false);

	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 135,"PROMISSES, DUE TO CREATION OF", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 150,"ARTIFICIAL BEINGS WORKERS.", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(2000);
	if (cut) { return; }

	graphLib.blank_area(0, 120, RES_W, 100);

	cut = graphLib.draw_progressive_text(10, 120,"BUT SOME INDIVIDUALS WANT TO USE", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 135,"THOSE ROBOTS FOR EVIL, CHANGING", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 150,"THEM TO STEAL AND HURT HUMANS.", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(2000);
	if (cut) { return; }


	graphLib.blank_area(0, 120, RES_W, 100);

	cut = graphLib.draw_progressive_text(10, 120,"WHILE A NEW KIND OF ROBOT RAISED,", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 135,"AND SAVED TOKYO, THE CITY OF", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 150,"KYOTO LIES DEFENSELESS.", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(2000);
	if (cut) { return; }

	graphLib.blank_area(0, 120, RES_W, 100);


	graphLib.blank_screen();

	graphLib.set_colormap(-1);

	_draw_text_thread = SDL_CreateThread(draw_threaded_text, this);

	for (int i=RES_W; i>0; i-=2) {
		graphLib.blank_area(0, 0, 320, 100, graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 10, INTRO_SURFACES[INTRO_SURFACES_CAPSULES].width, 90), st_position(i, 0), &INTRO_SURFACES[INTRO_SURFACES_CAPSULES], &graphLib.gameScreen);
		graphLib.updateScreen();
		input.waitScapeTime(10);
	}
	SDL_WaitThread(_draw_text_thread, NULL);
	cut = input.waitScapeTime(2000);
	if (cut) { return; }

	graphLib.blank_screen();
	graphLib.copyArea(st_rectangle(0, 0, INTRO_SURFACES[INTRO_SURFACES_LAB_BG].gSurface->w, INTRO_SURFACES[INTRO_SURFACES_LAB_BG].gSurface->h), st_position(10-graphLib.RES_DIFF_W, 10-graphLib.RES_DIFF_H), &INTRO_SURFACES[INTRO_SURFACES_LAB_BG], &graphLib.gameScreen);

	graphLib.copyArea(st_rectangle(0, 0, 21, 24), st_position(226-graphLib.RES_DIFF_W, 82), &INTRO_SURFACES[INTRO_SURFACES_KANOTUS], &graphLib.gameScreen);


	graphLib.updateScreen();
	graphLib.set_colormap(-3);


	cut = graphLib.draw_progressive_text(10, 120,"AFTER A PROTOTYPE WAS BUILT,", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 135,"HE PERFECTED THE PROJECT INTO ", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 150,"A SECOND ROBOT, JUST IN TIME,", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(500);
	if (cut) { return; }

	graphLib.copyArea(st_rectangle(43, 0, 21, INTRO_SURFACES[INTRO_SURFACES_KANOTUS].gSurface->h), st_position(226-graphLib.RES_DIFF_W, 82), &INTRO_SURFACES[INTRO_SURFACES_KANOTUS], &graphLib.gameScreen);

	cut = graphLib.draw_progressive_text(10, 165,"AS AN EVIL HORDE OF ROBOTS", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 180,"ATTACKED THE HOPELESS CITY.", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(2000);
	if (cut) { return; }

	graphLib.blank_area(0, 120, RES_W, 100);

	graphLib.clear_area(10, 10, 300, 100, 27, 63, 95);
	graphLib.copyArea(st_position(220, 44), &INTRO_SURFACES[INTRO_SURFACES_KANOTUS_BIG], &graphLib.gameScreen);
	graphLib.copyArea(st_position(30, 47), &INTRO_SURFACES[INTRO_SURFACES_BETABOT_BIG], &graphLib.gameScreen);
	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_FULL_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();


	//graphLib.copyArea(st_rectangle(88, 9, 19, 24), st_position(158-graphLib.RES_DIFF_W, 83), &INTRO_SURFACES[INTRO_SURFACES_BETABOT], &graphLib.gameScreen);

	cut = graphLib.draw_progressive_text(10, 120,"NOW THE CITY'S ONLY DEFENSE", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 135,"FROM THIS ATTACKING MINIONS", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

	cut = graphLib.draw_progressive_text(10, 150,"ARE THOSE TWO ROBOTS...", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(100);
	if (cut) { return; }

#define TIME_SHORT 120
#define TIME_LONG 300

	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_HALF_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(TIME_SHORT);
	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_FULL_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(TIME_LONG);

	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_HALF_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(TIME_LONG);
	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_FULL_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(TIME_SHORT);

	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG_HALF_SLEEP], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(TIME_LONG);
	graphLib.copyArea(st_position(80, 47), &INTRO_SURFACES[INTRO_SURFACES_ROCKBOT_BIG], &graphLib.gameScreen);
	graphLib.updateScreen();
	input.waitScapeTime(2000);


	cut = graphLib.draw_progressive_text(10, 165,"BETABOT AND ROCKBOT!!!", true, false);
	if (cut) { return; }
	cut = input.waitScapeTime(2000);
	if (cut) { return; }
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::intro_presents() {

	graphLib.blank_screen();
	graphLib.updateScreen();

	graphLib.copyArea(st_rectangle(0, 0, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS].width, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS].height), st_position(RES_W*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS].width*0.5, RES_H*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS].height*0.5), &INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS], &graphLib.gameScreen);
	//copy_area(INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS], 0, 0, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->w, INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->h, RES_W*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->w*0.5, RES_H*0.5-INTRO_SURFACES[INTRO_SURFACES_UPPERLAND_PRESENTS]->h*0.5, game_screen);
	graphLib.updateScreen();
	input.waitScapeTime(1600);

	graphLib.blank_screen();
}


// ********************************************************************************************** //
// mostra tela de introdução, até alguém apertar start/enter
// a partir daí, mostra tela de seleção de personagem
// ********************************************************************************************** //
void scenesLib::main_screen() {
	graphLib.blank_screen();
	graphLib.updateScreen();

	// PARTE 1 - TITLE SCREEN
	graphLib.copyArea(st_position(-graphLib.RES_DIFF_W, -graphLib.RES_DIFF_H), &INTRO_SURFACES[INTRO_SURFACES_INTRO_SCREEN], &graphLib.gameScreen);

	graphLib.draw_text(8, 50,"KYOTO EVOLUTION!!");

	std::vector<std::string> options;
	options.push_back("New Game");
	options.push_back("Load Game");
	options.push_back("Options");
	options.push_back("Credits");
	options.push_back("Quit");

	option_picker main_picker(false, st_position(40-graphLib.RES_DIFF_W, (RES_H*0.5)-graphLib.RES_DIFF_H), options);

	graphLib.draw_text(40-graphLib.RES_DIFF_W, (RES_H-30),"(C) 2009/2010 UPPERLAND STUDIO");
	graphLib.draw_text(40-graphLib.RES_DIFF_W, (RES_H-20),"DESIGN GREENNEKO WEB&DESIGN");
	graphLib.draw_text(40-graphLib.RES_DIFF_W, (RES_H-10),"AND OTHERS");

	format_v_2_0_1::file_io fio;

	int picked_n=1;
	if (fio.save_exists()) {
		main_picker.set_picker_initial_pos(1);
	}
	bool repeat_menu = true;
	while (repeat_menu == true) {
		picked_n = main_picker.pick();
		if (picked_n == -1) {
			exit(-1);
		} else if (picked_n == 0) {
			repeat_menu = false;
		} else if (picked_n == 1) {
			if (fio.save_exists()) {
				fio.read_save(game_save);
				repeat_menu = false;
			}
		} else if (picked_n == 2) {
			//config_showMainConfig();
		} else if (picked_n == 3) {
			showCredits();
		} else if (picked_n == 4) {
			exit(-1);
		}
	}

	//mainMenu();
	graphLib.updateScreen();


	// TODO - select_load_game();
	game_config.selected_player = select_player();
	// TODO - select_player();

}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
struct st_position scenesLib::pick_stage() {
	graphLib.blank_screen();
	stage_select selection(STAGE_SELECT_SURFACES);

	selected_stage = selection.select();

    //std::cout << "scenesLib::pick_stage - c.x: " << selected_stage.x << ", c.y: " << selected_stage.y << std::endl;

	int pos_n = selected_stage.x + 1 + selected_stage.y*3;
	//printf(">> pick_stage - pos.x: %d, pos.y: %d, pos_n: %d, save_001.stages[pos_n]: %d\n", pos.x, pos.y, pos_n, save_001.stages[pos_n]);
	// TODO
	//if (pos_n == SKULLCASTLE1 || save_001.stages[pos_n] == 0) {
	soundManager.stop_music();
	graphLib.boss_intro(pos_n);
	//}
	input.waitTime(100);
	return selected_stage;
}

// ********************************************************************************************** //
//  generic function to pick an option, shows a cursor indicating the current one,                //
// and accepts up/down input                                                                      //
// ********************************************************************************************** //
int scenesLib::option_picker_old(st_position initial_point, int option_number) {
	bool fim = false;
	int picked = 0;
	input.clean();
	input.waitTime(100);

	//cout << "scenesLib::option_picker::START\n";
	graphLib.drawCursor(st_position(initial_point.x, initial_point.y));

	while (!fim) {
		input.readInput();
		if (input.p1_input[BTN_START]) {
			//cout << "scenesLib::option_picker::END\n";
			return picked;
		}
		if (input.p1_input[BTN_DOWN]) {
			soundManager.play_sfx(SFX_CURSOR);
			graphLib.eraseCursor(st_position(initial_point.x, initial_point.y+(picked*CURSOR_SPACING)));
			picked++;
			if (picked >= option_number) {
				picked = 0;
			}
			graphLib.drawCursor(st_position(initial_point.x, initial_point.y+(picked*CURSOR_SPACING)));
		}
		if (input.p1_input[BTN_UP]) {
			soundManager.play_sfx(SFX_CURSOR);
			graphLib.eraseCursor(st_position(initial_point.x, initial_point.y+(picked*CURSOR_SPACING)));
			picked--;
			if (picked <= 0) {
				picked = option_number-1;
			}
			graphLib.drawCursor(st_position(initial_point.x, initial_point.y+(picked*CURSOR_SPACING)));
		}
		input.clean();
		input.waitTime(10);
		graphLib.updateScreen();
	}
	return picked;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void scenesLib::mainMenu()
{
	bool fim = false;
	st_position initial_pos(28, 100);
	int picked = 0;

	while (!fim) {
		picked = option_picker_old(initial_pos, 4);
		if (picked == 0) {
			fim = true;
		} else if (picked == 1) {
			//config_showMainConfig();
		} else if (picked == 2) {
			//showCredits();
		} else if (picked == 3) {
			//leaveGame();
		}
	}
}


void scenesLib::showCredits(){
	graphLib.show_config_bg(0);
    graphLib.draw_text(40, 30, "Rockbot v 1.0");
    graphLib.draw_text(40, 40, "Code: GPL v2");
    graphLib.draw_text(40, 50, "Images: C.C. Share Alike");
    graphLib.draw_text(40, 60, "Music: several licenses");
    graphLib.draw_text(40, 80, "Lead Game Developer:");
    graphLib.draw_text(40, 90, "- Iuri Fiedoruk");
    graphLib.draw_text(40, 100, "Lead Design Developer:");
    graphLib.draw_text(40, 110, "- Arismeire K. S. Fiedoruk");
    graphLib.draw_text(40, 120, "Helper Coders:");
    graphLib.draw_text(40, 130, "- Demetrio Neto");
    graphLib.draw_text(40, 140, "Helper Artists:");
    graphLib.draw_text(40, 150, "- Rodrigo Hahn");
    graphLib.draw_text(40, 160, "- Akira Faganello");
    graphLib.updateScreen();
    input.clean();
    input.waitTime(300);
    input.wait_keypress();
	graphLib.hide_config_bg(0);
}

void scenesLib::draw_lights_select_player(graphicsLib_gSurface& lights, int selected, int adjustX, int adjustY) {
	int posX, invPosX;

	posX = 6;
	invPosX = 0;

	//printf(">>draw_lights_select_player - game_config.selected_player: %d, adjustX: %d, adjustY: %d\n", game_config.selected_player, adjustX, adjustY);

	if (selected == 2) {
		// light for p1
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+2), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+88), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+2), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+88), &lights, &graphLib.gameScreen);
		// black for p2
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+98), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+184), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+98), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+184), &lights, &graphLib.gameScreen);
	} else {
		// light for p2
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+98), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+184), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+98), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(posX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+184), &lights, &graphLib.gameScreen);
		// black for p1
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+2), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+2, adjustY+88), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+2), &lights, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(invPosX, 0, lights.height, lights.height), st_position(adjustX+248, adjustY+88), &lights, &graphLib.gameScreen);
	}
	graphLib.updateScreen();
}



unsigned short int scenesLib::select_player() {
	int adjustX, adjustY;
	int selected = 2;

	graphLib.blank_screen();
	adjustX = (RES_W-INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG].width)*0.5;
	adjustY = (RES_H-INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG].height)*0.5;
	//copy_area(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG], 0, 0, INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->w, INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG]->h, adjustX, adjustY, game_screen);
	graphLib.copyArea(st_position(adjustX, adjustY), &INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_BG], &graphLib.gameScreen);
	graphLib.updateScreen();

	input.clean();
	input.waitTime(200);
	while (input.p1_input[BTN_START] != 1 && input.p2_input[BTN_START] != 1) {
		input.readInput();
		if (selected == 2 && (input.p1_input[BTN_DOWN] == 1 || input.p1_input[BTN_DOWN] == 1)) {
			soundManager.play_sfx(SFX_CURSOR);
			selected = 1;
		} else if (selected == 1 && (input.p1_input[BTN_UP] == 1 || input.p1_input[BTN_UP] == 1)) {
			soundManager.play_sfx(SFX_CURSOR);
			selected = 2;
		} else if (input.p1_input[BTN_QUIT] == 1 || input.p1_input[BTN_QUIT] == 1) {
			exit(-1);
		}
		draw_lights_select_player(INTRO_SURFACES[INTRO_SURFACES_PLAYER_SELECT_LIGHTS], selected, adjustX, adjustY);
	}
    //printf(">> select_player.END, n: %d\n", selected);
	return selected;
}
