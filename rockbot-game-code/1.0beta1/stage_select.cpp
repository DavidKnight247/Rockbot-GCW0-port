#include "stage_select.h"

#include <string>

#include "file/format.h"

// INTERNAL GLOBALS
extern timerLib timer;
extern inputLib input;
extern soundLib soundManager;
extern graphicsLib graphLib;
extern std::string FILEPATH;
extern format_v_2_0_1::st_save game_save;
extern format_v1_0::st_game_config game_config;

extern format_v_2_0_1::file_game game_data;

stage_select::stage_select(graphicsLib_gSurface stage_ref[STAGE_SELECT_COUNT]) :
	highlight_animation_white(false), timer_intro(0), highlight_pos(135-graphLib.RES_DIFF_W, 78-graphLib.RES_DIFF_H)
{
	int i;
	for (i=0; i<STAGE_SELECT_COUNT; i++) {
		STAGE_SELECT_SURFACES[i] = &stage_ref[i];
	}
	load_graphics();
}

void stage_select::load_graphics() {
	std::string filename;
	filename = FILEPATH + "data/images/backgrounds/stage_select_highlighted.png";
	graphLib.surfaceFromFile(filename, &s_light);

	filename = FILEPATH + "data/images/backgrounds/stage_select_darkned.png";
	graphLib.surfaceFromFile(filename, &s_dark);

	if (game_config.selected_player == 1) {
		filename = FILEPATH + "data/images/faces/rockbot_eyes.png";
	} else {
		filename = FILEPATH + "data/images/faces/betabot_eyes.png";
	}

	graphLib.surfaceFromFile(filename, &eyes_surface);

	filename = FILEPATH + "data/images/backgrounds/stage_select.png";
	graphLib.surfaceFromFile(filename, &background);
}



void stage_select::move_highlight(short int x_inc, short int y_inc) {
	graphLib.copyArea(highlight_pos, &s_dark, &graphLib.gameScreen);
	highlight_pos.x += x_inc;
	highlight_pos.y += y_inc;
	graphLib.copyArea(highlight_pos, &s_light, &graphLib.gameScreen);
	graphLib.updateScreen();
}




//void stage_select::place_face(int face_n, char botname[20], short int posx, short int posy) {
void stage_select::place_face(std::string face_file, std::string botname, st_position pos) {
	// TODO - calculate the text position according to it's strlen
	int txtPosX, txtPosY;

	if (face_file.size() > 0) {
		graphLib.place_face(face_file, st_position(pos.x*80+63, pos.y*60+26));
	}
	txtPosX = pos.x*80+76-graphLib.RES_DIFF_W-(botname.size()*7)*0.5;
	txtPosY = pos.y*60+66-graphLib.RES_DIFF_H;
	graphLib.draw_text(txtPosX, txtPosY, botname);
}


bool stage_select::animate_highlight() {
	if (timer.getTimer() > timer_intro) {
		timer_intro = timer.getTimer()+200;
		if (highlight_animation_white == true) {
			return true;
		} else {
			return false;
		}
	} else {
		if (highlight_animation_white == true) {
			return false;
		} else {
			return true;
		}
	}
}

void stage_select::draw_eyes(int x, int y, bool erase_eyes) {
	int posX, posY;
	if (x == 1) {
		posX = 11;
	} else if (x == 0) {
		posX = 10;
	} else {
		posX = 12;
	}
	if (y == 1) {
		posY = 18;
	} else if (y == 0) {
		posY = 17;
	} else {
		posY = 19;
	}
	/*
	if (game_config.selected_player == 2) {
		posY -= 3;
	}
	*/
	posX = posX+80+63-graphLib.RES_DIFF_W;
	posY = posY+60+26-graphLib.RES_DIFF_H;
	if (erase_eyes) {
		if (game_config.selected_player == 1) {
			place_face("rockbot_no_eyes.png", "", st_position(1, 1));
		} else {
			place_face("betabot_no_eyes.png", "", st_position(1, 1));
		}
	} else {
		if (game_config.selected_player == 1) {
			graphLib.copyArea(st_position(posX, posY), &eyes_surface, &graphLib.gameScreen);
		} else {
			graphLib.copyArea(st_position(posX, posY-2), &eyes_surface, &graphLib.gameScreen);
		}
	}
}

struct st_position stage_select::select() {
	//SDL_Surface *spriteCopy, *s_light=NULL, *s_dark, *eyes_surface, *eyes_bg, *light_mode;
	struct st_position spacing, highlight_pos;
	int selection_end=0;

	soundManager.load_music("menu.mod");
	soundManager.play_music();
	input.clean();
	input.waitTime(300);

	select_pos.x = 1;
	select_pos.y = 1;
	spacing.x = 80;
	spacing.y = 60;
	highlight_pos.x = 135-graphLib.RES_DIFF_W;
	highlight_pos.y = 78-graphLib.RES_DIFF_H;
	graphLib.blank_screen();
	graphicsLib_gSurface *light_mode = &s_light;

	graphLib.copyArea(st_position(0, 0), &background, &graphLib.gameScreen);

	if (game_save.stages[APEBOT] == 0) {
		place_face(game_data.stages[APEBOT].boss.face_graphics_filename, "APE", st_position(0, 0));
	} else {
		place_face(std::string(""), "APE", st_position(0, 0));
	}

	if (game_save.stages[DAISIEBOT] == 0) {
		place_face(game_data.stages[DAISIEBOT].boss.face_graphics_filename, "DAISIE", st_position(1, 0));
	} else {
		place_face(std::string(""), "DAISIE", st_position(1, 0));
	}

	if (game_save.stages[SEAHORSEBOT] == 0) {
		place_face(game_data.stages[SEAHORSEBOT].boss.face_graphics_filename, "SEAHORSE", st_position(2, 0));
	} else {
		place_face(std::string(""), "SEAHORSE", st_position(2, 0));
	}

	if (game_save.stages[MUMMYBOT] == 0) {
		place_face(game_data.stages[MUMMYBOT].boss.face_graphics_filename, "MUMMY", st_position(0, 1));
	} else {
		place_face(std::string(""), "MUMMY", st_position(0, 1));
	}

	if (game_save.stages[MAGEBOT] == 0) {
		place_face(game_data.stages[MAGEBOT].boss.face_graphics_filename, "MAGE", st_position(2, 1));
	} else {
		place_face(std::string(""), "MAGE", st_position(2, 1));
	}

	if (game_save.stages[DYNAMITEBOT] == 0) {
		place_face(game_data.stages[DYNAMITEBOT].boss.face_graphics_filename, "DYNAMITE", st_position(0, 2));
	} else {
		place_face(std::string(""), "DYNAMITE", st_position(0, 2));
	}

	if (game_save.stages[SPIKEBOT] == 0) {
		place_face(game_data.stages[SPIKEBOT].boss.face_graphics_filename, "SPIKE", st_position(1, 2));
	} else {
		place_face(std::string(""), "SPIKE", st_position(1, 2));
	}

	if (game_save.stages[TECHNOBOT] == 0) {
		place_face(game_data.stages[TECHNOBOT].boss.face_graphics_filename, "TECHNO", st_position(2, 2));
	} else {
		place_face(std::string(""), "TECHNO", st_position(2, 2));
	}

	if (game_save.finished_stages < 9) {
		if (game_config.selected_player == 1) {
			place_face("rockbot_no_eyes.png", "", st_position(1, 1));
		} else {
			place_face("betabot_no_eyes.png", "", st_position(1, 1));
		}
	} else {
		place_face("dr_destrin.png", "Dr. D.", st_position(1, 1));
	}

	light_mode = &s_light;

	graphLib.copyArea(st_position(highlight_pos.x, highlight_pos.y), &s_light, &graphLib.gameScreen);
	graphLib.updateScreen();
	graphLib.draw_text(RES_W*0.5-45-graphLib.RES_DIFF_W, 5-graphLib.RES_DIFF_H,"PRESS START");

	input.clean();
	if (game_save.finished_stages < 9) {
		draw_eyes(select_pos.x, select_pos.y, false);
	}
	input.waitTime(200);

	while (selection_end == 0) {
		input.readInput();

		if (input.p1_input[BTN_QUIT] || input.p2_input[BTN_QUIT]) {
			exit(-1);
		}

		if (select_pos.y < 2 && (input.p1_input[BTN_DOWN] || input.p2_input[BTN_DOWN])) {
					draw_eyes(select_pos.x, select_pos.y, true);
					select_pos.y++;
					if (game_save.finished_stages < 9) {
						draw_eyes(select_pos.x, select_pos.y, false);
					}
					soundManager.play_sfx(SFX_CURSOR);
					move_highlight(0, spacing.y);
					input.waitTime(200);
					input.clean();
		} else if (select_pos.y > 0 && (input.p1_input[BTN_UP] || input.p2_input[BTN_UP])) {
					draw_eyes(select_pos.x, select_pos.y, true);
					select_pos.y--;
					if (game_save.finished_stages < 9) {
						draw_eyes(select_pos.x, select_pos.y, false);
					}
					soundManager.play_sfx(SFX_CURSOR);
					move_highlight(0, -spacing.y);
					input.waitTime(200);
					input.clean();
		} else if (select_pos.x > 0 && (input.p1_input[BTN_LEFT] || input.p2_input[BTN_LEFT])) {
					draw_eyes(select_pos.x, select_pos.y, true);
					select_pos.x--;
					if (game_save.finished_stages < 9) {
						draw_eyes(select_pos.x, select_pos.y, false);
					}
					soundManager.play_sfx(SFX_CURSOR);
					move_highlight(-spacing.x, 0);
					input.waitTime(200);
					input.clean();
		} else if (select_pos.x < 2 && (input.p1_input[BTN_RIGHT] || input.p2_input[BTN_RIGHT])) {
					draw_eyes(select_pos.x, select_pos.y, true);
					select_pos.x++;
					if (game_save.finished_stages < 9) {
						draw_eyes(select_pos.x, select_pos.y, false);
					}
					soundManager.play_sfx(SFX_CURSOR);
					move_highlight(spacing.x, 0);
					input.waitTime(200);
					input.clean();
		} else if ((input.p1_input[BTN_START] || input.p2_input[BTN_START]) && game_save.finished_stages < 9 && (select_pos.x != 1 || select_pos.y != 1)) {
			selection_end = 1;
		} else if ((input.p1_input[BTN_START] || input.p2_input[BTN_START]) && game_save.finished_stages >= 9) {
			selection_end = 1;
		}
		if (animate_highlight() == false) {
			light_mode = &s_light;
		} else {
			light_mode = &s_dark;
		}
		move_highlight(0, 0);
	}

    //std::cout << "stage_select::select - c.x: " << select_pos.x << ", c.y: " << select_pos.y << std::endl;

	graphLib.blink_screen(255, 255, 255);
	return select_pos;
}
