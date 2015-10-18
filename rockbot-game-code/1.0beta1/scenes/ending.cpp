#include "ending.h"

extern std::string FILEPATH;

#include "soundlib.h"
extern soundLib soundManager;

extern graphicsLib graphLib;

#include "inputlib.h"
extern inputLib input;

#include "soundlib.h"
extern soundLib soundManager;

#include "timerlib.h"
extern timerLib timer;

#include "game.h"
extern game gameControl;


//#include "graphic/option_picker.h"
#include "dialogs.h"

extern format_v1_0::st_game_config game_config;


ending::ending()
{
	// preload train
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/backgrounds/ending_layer_train.png"), &_train_surface);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/backgrounds/ending_layer_background.png"),& _bg_surface);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/backgrounds/ending_layer_fence.png"), &_fence_surface);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/backgrounds/ending_layer_bushes.png"), &_bushes_surface);
}

/// @TODO: player position is wrong for some reason (must reset scrolling?)
void ending::start()
{
	boss();
	train();
	if (game_config.selected_player == PLAYER_ROCKBOT) {
		lab();
	}
	bosses();
	credits();
}

void ending::boss()
{
	/// @TODO - get boss position from gameControl
	dialogs dialogs_obj;
	graphicsLib_gSurface last_stage_bg;
	graphicsLib_gSurface destrin_sprites;

	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/backgrounds/last_stage_bg.png"), &last_stage_bg);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/sprites/destrin.png"), &destrin_sprites);
	graphLib.showSurface(&last_stage_bg);
	graphLib.updateScreen();
	st_position p_pos = gameControl.get_player_position(0);
	st_size p_size = gameControl.get_player_size(0);


	// fall to ground
	if (p_pos.y+p_size.heigth < 191) {
		gameControl.set_player_anim_type(ANIM_TYPE_JUMP, 0);
		while (p_pos.y+p_size.heigth < 191) {
			if (p_pos.y+p_size.heigth + 6 <= 191) {
				p_pos.y += 6;
			} else {
				p_pos.y += 1;
			}
			gameControl.set_player_position(p_pos, 0);
			graphLib.showSurface(&last_stage_bg);
			gameControl.show_player(0);
			graphLib.updateScreen();
			input.waitTime(20);
		}
	}

	gameControl.set_player_anim_type(ANIM_TYPE_WALK, 0);

	gameControl.set_player_anim_type(ANIM_TYPE_WALK, 1);
	gameControl.set_player_position(st_position(RES_W+TILESIZE*3, 159), 1);
	st_position p_pos2 = gameControl.get_player_position(1);
	gameControl.set_player_direction(ANIM_DIRECTION_LEFT, 1);

	if (p_pos.x > RES_W/3) {
		gameControl.set_player_direction(ANIM_DIRECTION_LEFT, 0);
		while (p_pos.x > RES_W/3) {
			p_pos.x -= 3;
			gameControl.set_player_position(p_pos, 0);
			graphLib.showSurface(&last_stage_bg);
			gameControl.show_player(0);
			graphLib.updateScreen();
			input.waitTime(20);
		}
	} else {
		gameControl.set_player_direction(ANIM_DIRECTION_RIGHT, 0);
		while (p_pos.x < RES_W/3) {
			p_pos.x += 3;
			gameControl.set_player_position(p_pos, 0);
			graphLib.showSurface(&last_stage_bg);
			gameControl.show_player(0);
			graphLib.updateScreen();
			input.waitTime(20);
		}
	}

	gameControl.set_player_anim_type(ANIM_TYPE_STAND, 0);
	graphLib.showSurface(&last_stage_bg);
	gameControl.show_player(0);
	graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
	graphLib.updateScreen();
	input.waitTime(20);

	std::string lines[3];

	if (game_config.selected_player == PLAYER_ROCKBOT) {
		gameControl.set_player_position(st_position(RES_W/3+TILESIZE*3, -TILESIZE*3), 1);
		st_position p_pos2 = gameControl.get_player_position(1);
		lines[0] = "I CAN'T BELIEVE IT!!";
		lines[1] = "YOU SHOULD NOT BE ABLE";
		lines[2] = "TO DEFEAT ME.";
		dialogs_obj.show_timed_dialog("destrin.png", true, lines, 2000);

		lines[0] = "YOUR EGO IS YOUR.";
		lines[1] = "WEAKNESS, AND YOUR";
		lines[2] = "PLACE IS PRISION.";
		dialogs_obj.show_timed_dialog("rockbot.png", true, lines, 2000);

		gameControl.set_player_anim_type(ANIM_TYPE_TELEPORT, 1);
		while (p_pos2.y < 159) {
			if (p_pos2.y+8 <= 159) {
				p_pos2.y += 8;
			} else {
				p_pos2.y += 1;
			}
			gameControl.set_player_position(p_pos2, 1);
			graphLib.showSurface(&last_stage_bg);
			gameControl.show_player(1);
			gameControl.show_player(0);
			graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
			graphLib.updateScreen();
			input.waitTime(20);
		}

		gameControl.set_player_anim_type(ANIM_TYPE_STAND, 1);
		graphLib.showSurface(&last_stage_bg);
		gameControl.show_player(1);
		gameControl.show_player(0);
		graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
		graphLib.updateScreen();
		input.waitTime(20);

		lines[0] = "DESTROY HIM, ROCK,";
		lines[1] = "OR HE WILL KEEP";
		lines[2] = "HURTING HUMANS!";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		lines[0] = "NO BETA, THIS";
		lines[1] = "ISN'T HOW WE DO.";
		lines[2] = "";
		dialogs_obj.show_timed_dialog("rockbot.png", true, lines, 2000);

		lines[0] = "VERY WELL, THEN";
		lines[1] = "YOU MUST KNOW";
		lines[2] = "I'M NOT WITH YOU!";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		lines[0] = "THIS IS NOT THE END.";
		lines[1] = "WE'LL MEET AGAIN";
		lines[2] = "ROCK.. AND BETA.";
		dialogs_obj.show_timed_dialog("destrin.png", true, lines, 2000);

	} else if (PLAYER_BETABOT) {
		lines[0] = "THIS IS IMPOSSIBLE!!";
		lines[1] = "YOU ARE TOO STRONG,";
		lines[2] = "YOU ARE A MONSTER!";
		dialogs_obj.show_timed_dialog("destrin.png", true, lines, 2000);

		lines[0] = "I AM, AS STRONG AS";
		lines[1] = "NEEDED TO PROTECT.";
		lines[2] = "MAKE YOUR PRAYERS.";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		gameControl.set_player_anim_type(ANIM_TYPE_ATTACK, 0);
		graphLib.showSurface(&last_stage_bg);
		gameControl.show_player(0);
		graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
		graphLib.updateScreen();
		input.waitTime(20);

		lines[0] = "NO, ROBOTS CAN'T.";
		lines[1] = "HARM HUMANS!";
		lines[2] = "STOP.";
		dialogs_obj.show_timed_dialog("destrin.png", true, lines, 2000);

		lines[0] = "I'M MORE THAN A";
		lines[1] = "SIMPLE 'ROBOT'.";
		lines[2] = "NOW DIE!";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		lines[0] = "WAIT BETA!";
		lines[1] = "DON'T DO IT.";
		lines[2] = "";
		dialogs_obj.show_timed_dialog("rockbot.png", true, lines, 2000);

		while (p_pos2.x > RES_W/3+TILESIZE*3) {
			p_pos2.x -= 3;
			gameControl.set_player_position(p_pos2, 1);
			graphLib.showSurface(&last_stage_bg);
			graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
			gameControl.show_player(0);
			gameControl.show_player(1);
			graphLib.updateScreen();
			input.waitTime(20);
		}
		gameControl.set_player_anim_type(ANIM_TYPE_SHIELD, 1);
		graphLib.showSurface(&last_stage_bg);
		graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
		gameControl.show_player(0);
		gameControl.show_player(1);
		graphLib.updateScreen();
		input.waitTime(1000);

		lines[0] = "ROCK... YOU ARE";
		lines[1] = "PROTECTING THIS..";
		lines[2] = "VILAIN!?";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		lines[0] = "YES, BECAUSE IF";
		lines[1] = "YOU KILL HIM,";
		lines[2] = "PEOPLE WILL HATE YOU.";
		dialogs_obj.show_timed_dialog("rockbot.png", true, lines, 2000);

		gameControl.set_player_anim_type(ANIM_TYPE_STAND, 0);
		gameControl.set_player_anim_type(ANIM_TYPE_SHIELD, 1);
		graphLib.showSurface(&last_stage_bg);
		graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
		gameControl.show_player(0);
		gameControl.show_player(1);
		graphLib.updateScreen();

		lines[0] = "RIGHT. BUT REMEMBER,";
		lines[1] = "ANYONE HE HURTS,";
		lines[2] = "IS ON YOU.";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		lines[0] = "I KNOW.";
		lines[1] = "WHERE WILL YOU GO?";
		lines[2] = "";
		dialogs_obj.show_timed_dialog("rockbot.png", true, lines, 2000);

		lines[0] = "FOLLOW THE WIND,";
		lines[1] = "TO BE FORGOTTEN,";
		lines[2] = "TO BE FREE.";
		dialogs_obj.show_timed_dialog("betabot.png", true, lines, 2000);

		gameControl.set_player_anim_type(ANIM_TYPE_TELEPORT, 0);
		gameControl.set_player_anim_type(ANIM_TYPE_STAND, 1);

		while (p_pos.y > -TILESIZE*3) {
			p_pos.y -= 8;
			gameControl.set_player_position(p_pos, 0);
			graphLib.showSurface(&last_stage_bg);
			graphLib.showSurfaceRegionAt(&destrin_sprites, st_rectangle(44, 0, 20, destrin_sprites.height), st_position(250, 167));
			gameControl.show_player(0);
			gameControl.show_player(1);
			graphLib.updateScreen();
			input.waitTime(20);
		}
	}

}

void ending::train()
{
	int end_timer;
	int pos_layer_bg=0;
	int pos_layer_fence=0;
	int check_timer;
	int train_move=0;
	int played_sfx = 0;
	int posx_adjust = 0;
	int pos_bushes_1 = 0;
	int pos_bushes_2 = 0;


	graphLib.set_colormap(-1);


	soundManager.stop_music();
	soundManager.load_music("train.mod");
	soundManager.play_music();


	pos_bushes_1 = RES_W + _bushes_surface.width;
	pos_bushes_2 = RES_W*1.5 + _bushes_surface.width;


	graphLib.copyArea(st_rectangle(0, 0, RES_W, _bg_surface.height), st_position(0, 0), &_bg_surface, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(0, 0, RES_W, _fence_surface.height), st_position(0, _bg_surface.height), &_fence_surface, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(0, 0, RES_W, _train_surface.height), st_position(0, _bg_surface.height+_fence_surface.height), &_train_surface, &graphLib.gameScreen);
	graphLib.updateScreen();


	end_timer = timer.getTimer()+10000;
	check_timer = timer.getTimer()+2000;
	graphLib.blank_screen();

	gameControl.set_player_anim_type(ANIM_TYPE_STAND, 0);
	gameControl.set_player_direction(ANIM_DIRECTION_RIGHT, 0);
	gameControl.set_player_position(st_position(RES_W/6, _bg_surface.height+_fence_surface.height - gameControl.get_player_size(0).heigth), 0);
	st_position p_pos = gameControl.get_player_position(0);


	while (timer.getTimer() < end_timer) {
		// move background
		pos_layer_bg++;
		if (pos_layer_bg >= _bg_surface.width-RES_W) {
			pos_layer_bg = 0;
		}
		// move fence
		pos_layer_fence += 4;
		if (pos_layer_fence >= _fence_surface.width-RES_W) {
			pos_layer_fence = 0;
		}

		pos_bushes_1 -= 12;
		if (pos_bushes_1 < -_bushes_surface.width) {
			pos_bushes_1 = RES_W + _bushes_surface.width;
		}

		pos_bushes_2 -= 12;
		if (pos_bushes_2 < -_bushes_surface.width) {
			pos_bushes_2 = RES_W + _bushes_surface.width;
		}


		graphLib.copyArea(st_rectangle(pos_layer_bg, 0, RES_W, _bg_surface.height), st_position(0, 0), &_bg_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _bg_surface.height), st_position(0, 0), &_bg_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(pos_layer_fence, 0, RES_W, _fence_surface.height), st_position(0, _bg_surface.height), &_fence_surface, &graphLib.gameScreen);

		if (timer.getTimer() > check_timer) {
			if (train_move == 0) {
				train_move = 1;
				check_timer = timer.getTimer()+100;
			} else {
				train_move = 0;
				check_timer = timer.getTimer()+2000;
			}
		}
		if (train_move == 0) {
			posx_adjust = 0;
			played_sfx = 0;
		} else {
			if (!played_sfx) {
				soundManager.play_sfx(SFX_TRAIN_CHUNK);
			}
			played_sfx = 1;
			posx_adjust = 2;
		}

		graphLib.copyArea(st_rectangle(posx_adjust, 0, RES_W, _train_surface.height), st_position(0, _bg_surface.height+_fence_surface.height), &_train_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(pos_layer_fence, 0, RES_W, _fence_surface.height), st_position(0, _bg_surface.height), &_fence_surface, &graphLib.gameScreen);


		gameControl.set_player_position(st_position(p_pos.x - posx_adjust, p_pos.y), 0);
		gameControl.show_player(0);

		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_1, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_2, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);

		graphLib.updateScreen();

		input.waitTime(30);
	}

	short int walk_total = p_pos.x + RES_W/2;

	// walk to the right-side of screen
	gameControl.set_player_anim_type(ANIM_TYPE_WALK, 0);
	while (p_pos.x < walk_total) {
		p_pos.x += 3;
		gameControl.set_player_position(st_position(p_pos), 0);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _bg_surface.height), st_position(0, 0), &_bg_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _fence_surface.height), st_position(0, _bg_surface.height), &_fence_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _train_surface.height), st_position(0, _bg_surface.height+_fence_surface.height), &_train_surface, &graphLib.gameScreen);
		gameControl.show_player(0);
		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_1, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_2, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);
		graphLib.updateScreen();
		input.waitTime(20);
	}

	// jump up
	float jump_speed = 0.0;
	gameControl.set_player_anim_type(ANIM_TYPE_JUMP, 0);
	int jump_up = 1;
	float acceleration = 0.12;
	short max_speed = 9;
	jump_speed = max_speed;

	while (p_pos.y < RES_H) { // @TODO: get_player_size, use height
		if (jump_up == 1) {
			jump_speed -= jump_speed*acceleration;
			p_pos.y -= jump_speed;
		} else {
			if (jump_speed <= 0) {
				jump_speed = 1;
			}
			jump_speed += jump_speed*acceleration;
			p_pos.y += jump_speed;
		}
		gameControl.set_player_position(st_position(p_pos), 0);

		graphLib.copyArea(st_rectangle(0, 0, RES_W, _bg_surface.height), st_position(0, 0), &_bg_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _fence_surface.height), st_position(0, _bg_surface.height), &_fence_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, RES_W, _train_surface.height), st_position(0, _bg_surface.height+_fence_surface.height), &_train_surface, &graphLib.gameScreen);
		gameControl.show_player(0);
		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_1, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, _bushes_surface.width, _bushes_surface.height), st_position(pos_bushes_2, RES_H-_bushes_surface.height), &_bushes_surface, &graphLib.gameScreen);
		graphLib.updateScreen();

		if (jump_up == 1 && jump_speed <= 1.0) {
			jump_up = 0;
		}

		input.waitTime(20);
	}
}

void ending::lab()
{
	graphicsLib_gSurface lab_bg, kanotus_surface, candybot_surface;
	st_position p_pos;

	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/scenes/lab_ending.png"), &lab_bg);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/sprites/canotus.png"), &kanotus_surface);
	graphLib.surfaceFromFile(std::string(FILEPATH+"data/images/sprites/candybot_ending.png"), &candybot_surface);

	graphLib.blank_screen();
	graphLib.set_colormap(-3);
	graphLib.copyArea(st_rectangle(0, 0, lab_bg.width, lab_bg.height), st_position(0, 10-graphLib.RES_DIFF_H), &lab_bg, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(0, 0, 21, 24), st_position(58, 82), &kanotus_surface, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(0, 0, 16, 26), st_position(174, 80), &candybot_surface, &graphLib.gameScreen);

	gameControl.set_player_position(st_position(RES_W+TILESIZE*2, 74), 0);
	gameControl.set_player_direction(ANIM_DIRECTION_LEFT, 0);
	gameControl.set_player_anim_type(ANIM_TYPE_WALK, 0);
	p_pos = gameControl.get_player_position(0);

	while (p_pos.x > RES_W/2-50) {
		p_pos.x -= 3;
		graphLib.copyArea(st_rectangle(0, 0, lab_bg.width, lab_bg.height), st_position(0, 10-graphLib.RES_DIFF_H), &lab_bg, &graphLib.gameScreen);
		graphLib.copyArea(st_rectangle(0, 0, 21, 24), st_position(58, 82), &kanotus_surface, &graphLib.gameScreen);
		if (p_pos.x > 172) {
			graphLib.copyArea(st_rectangle(0, 0, 16, 26), st_position(174, 80), &candybot_surface, &graphLib.gameScreen);
		} else {
			graphLib.copyArea(st_rectangle(16, 0, 16, 26), st_position(174, 80), &candybot_surface, &graphLib.gameScreen);
		}
		gameControl.set_player_position(p_pos, 0);
		gameControl.show_player(0);
		graphLib.updateScreen();
		input.waitTime(20);
	}
	gameControl.set_player_anim_type(ANIM_TYPE_STAND, 0);
	graphLib.copyArea(st_rectangle(0, 0, lab_bg.width, lab_bg.height), st_position(0, 10-graphLib.RES_DIFF_H), &lab_bg, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(64, 0, 21, 24), st_position(58, 82), &kanotus_surface, &graphLib.gameScreen);
	graphLib.copyArea(st_rectangle(16, 0, 16, 26), st_position(174, 80), &candybot_surface, &graphLib.gameScreen);
	gameControl.show_player(0);
	graphLib.updateScreen();
	input.waitTime(1000);

	dialogs dialogs_obj;
	std::string lines[3];
	lines[0] = "ROCK, CONGRATULATIONS!";
	lines[1] = "YOU ARE THE CITY HERO";
	lines[2] = "AND PEOPLE LOVE YOU.";
	dialogs_obj.show_timed_dialog("candybot.png", true, lines, 2000);

	lines[0] = "I DREAMED ABOUT THIS.";
	lines[1] = "BUT NOW THAT HAPPENED,";
	lines[2] = "I AM SAD.";
	dialogs_obj.show_timed_dialog("rockbot_sad.png", true, lines, 2000);

	lines[0] = "WHY?";
	lines[1] = "AND WHERE IS BETA?";
	lines[2] = "";
	dialogs_obj.show_timed_dialog("candybot.png", true, lines, 2000);

	lines[0] = "HE...HE...";
	lines[1] = "HE DECIDED TO GO,";
	lines[2] = "HIS OWN WAY.";
	dialogs_obj.show_timed_dialog("rockbot_sad.png", true, lines, 2000);

	lines[0] = "HE IS FREE TO CHOOSE,";
	lines[1] = "AND WE WILL";
	lines[2] = "NEVER FORGET HIM.";
	dialogs_obj.show_timed_dialog("canotus_face.png", true, lines, 2000);

	graphLib.set_colormap(-1);
}

void ending::bosses()
{
	soundManager.stop_music();
	soundManager.load_music("ending_bosses.mod");
	soundManager.play_music();
	/// @TODO - copy from boss_intro
	graphLib.boss_credits(APEBOT);
	graphLib.boss_credits(TECHNOBOT);
	graphLib.boss_credits(DYNAMITEBOT);
	graphLib.boss_credits(DAISIEBOT);
	graphLib.boss_credits(SPIKEBOT);
	graphLib.boss_credits(MUMMYBOT);
	graphLib.boss_credits(SEAHORSEBOT);
	graphLib.boss_credits(MAGEBOT);

}

void ending::credits()
{
	st_rectangle dest;
	graphicsLib_gSurface credits_surface;

	int line_n=0, scrolled=0;
	int posY = -RES_H;

	soundManager.stop_music();
	soundManager.load_music("ending.mod");
	soundManager.play_music();

	graphLib.blank_screen();

	graphLib.updateScreen();
	graphLib.initSurface(st_size(RES_W, RES_H+12), &credits_surface);
	graphLib.blank_surface(credits_surface);

	// add the initial lines to screen
	draw_credits_text(credits_surface);

	graphLib.updateScreen();

	// scroll the lines
	while (scrolled < (credits_list.size()*12)+RES_H/2+18) {
		graphLib.copyArea(st_rectangle(0, posY, RES_W, RES_H), st_position(0, 0), &credits_surface, &graphLib.gameScreen);
		graphLib.updateScreen();
		posY++;
		scrolled++;
		if (posY > 12) {
			graphLib.copyArea(st_rectangle(0, posY, credits_surface.width, RES_H), st_position(0, 0), &credits_surface, &credits_surface);
			// scroll the lines
			dest.x = 0;
			dest.y = RES_H;
			dest.w = RES_W;
			dest.h = 12;
			graphLib.blank_area(dest.x, dest.y, dest.w, dest.h, credits_surface);
			draw_credit_line(credits_surface, line_n+18);
			posY = 0;
			line_n++;
		} else {
			input.waitTime(50);
		}
	}
	graphLib.updateScreen();
	input.wait_keypress();
}

void ending::draw_credits_text(graphicsLib_gSurface &surface)
{
	credits_list.push_back("- CHARACTER DESIGN -");
	credits_list.push_back("GIOVANI AKIRA FAGANELLO");
	credits_list.push_back("RODRIGO M. HAHN");
	credits_list.push_back("IURI FIEDORUK");
	credits_list.push_back("ARISMEIRE K.S.F");
	credits_list.push_back("");

	credits_list.push_back("- SOUND COMPOSE -");
	credits_list.push_back("MODARCHIVE.ORG");
	credits_list.push_back("");
	/// @TODO - individual credits

	credits_list.push_back("- PROGRAMMER -");
	credits_list.push_back("IURI FIEDORUK");
	credits_list.push_back("DEMETRIO NETO");
	credits_list.push_back("");

	credits_list.push_back("- PLANNER -");
	credits_list.push_back("IURI FIEDORUK");
	credits_list.push_back("ARISMEIRE K.S.F");
	credits_list.push_back("");

	credits_list.push_back("- SPECIAL THANKS -");
	credits_list.push_back("SDL LIBRARY PORTERS");
	credits_list.push_back("DEVIANTART COMMUNITY");
	credits_list.push_back("VENOM");
	credits_list.push_back("JERONIMO");
	credits_list.push_back("");

	credits_list.push_back("- MARKETING AND CONCEPT ART -");
	credits_list.push_back("ARISMEIRE K.S.F");
	credits_list.push_back("");

	credits_list.push_back("- PS2 PORTING -");
	credits_list.push_back("RAGNAROK2040");
	credits_list.push_back("WOON-YUNG LIU");
	credits_list.push_back("");

	credits_list.push_back("- DEVELOPMENT LIBRARIES -");
	credits_list.push_back("LIBSDL.ORG");
	credits_list.push_back("NOKIA Qt4");
	credits_list.push_back("PS2DEV.ORG");
	credits_list.push_back("");

	credits_list.push_back("- DEVELOPMENT TOOLS -");
	credits_list.push_back("UBUNTU LINUX");
	credits_list.push_back("GIMP");
	credits_list.push_back("QTCREATOR");
	credits_list.push_back("PAINT TOOL SAI");
	credits_list.push_back("COREL DRAW");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("");
	credits_list.push_back("- PRESENTED BY -");
	credits_list.push_back("UPPERLAND STUDIOS");
	credits_list.push_back("GREENNEKO WEB & DESIGN");

	for (int i=0; i<=17 && i<credits_list.size(); i++) {
		graphLib.draw_centered_text(12*i, credits_list.at(i), surface);
	}

}

void ending::draw_credit_line(graphicsLib_gSurface &surface, short initial_line)
{
	if (initial_line < credits_list.size()) {
		graphLib.draw_centered_text(17*12, credits_list.at(initial_line), surface);
	}
}
