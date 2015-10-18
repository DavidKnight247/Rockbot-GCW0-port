#include "game.h"
#include <fstream>

#include "timerlib.h"
extern timerLib timer;

#include <math.h>

#include "graphicslib.h"
extern graphicsLib graphLib;

#include "soundlib.h"
extern soundLib soundManager;

#include "inputlib.h"
extern inputLib input;

#include "file/format.h"

#include "defines.h"

#include "scenes/ending.h"
#include "file/file_io.h"

extern format_v_2_0_1::st_save game_save;

extern format_v1_0::st_game_config game_config;

extern struct format_v_2_0_1::st_checkpoint checkpoint;


extern string FILEPATH;

extern format_v_2_0_1::file_game game_data;

extern bool quick_load;

// ********************************************************************************************** //
// class constructor                                                                              //
// ********************************************************************************************** //
game::game() : loaded_stage(NULL)
{
	currentStage = 1;
	fps_timer =  timer.getTimer()+1000;
	fps_counter = 0;
	_frame_duration = 1000/60; // each frame must use this share of time
}

// ********************************************************************************************** //
// class destructor                                                                               //
// ********************************************************************************************** //
game::~game()
{
	// clean allocated stages
	delete loaded_stage;
}

// ********************************************************************************************** //
// initializar game, can't be on constructor because it needs other objects (circular)            //
// ********************************************************************************************** //
void game::initGame()
{
	stringstream player_name;
	player_name << "p" << game_config.selected_player;
	players.push_back(classPlayer(player_name.str(), game_config.selected_player));
	// always insert second player, as it is used in cutscenes and such
	if (game_config.selected_player == 1) {
		players.push_back(classPlayer("p2",2));
	} else {
		players.push_back(classPlayer("p1", 1));
	}
	config_manager.set_player_ref(&players.at(0));
	load_game_objects();
	load_map_links();
	if (loaded_stage != NULL) {
		delete loaded_stage;
	}
	loaded_stage = new stage(currentStage, game_object_list, players);
	players.at(0).set_is_player(true);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::showGame(bool can_characters_move)
{
	if (config_manager.execute_ingame_menu()) { // game is paused
		std::cout << "execute_ingame_menu" << std::endl;
		return;
	}

	//std::cout << "game::showGame" << std::endl;
	_t1 = timer.getTimer();

	if (loaded_stage->get_current_map() != players.at(0).map) {
		std::cout << "ERROR MAP INVALID (" << loaded_stage->get_current_map() << "), pllayer.map: " << players.at(0).map << std::endl;
		exit(-1);
	}

	if (players.at(0).is_dead() == true) {
		restart_stage();
		return;
	}

	if (test_teleport(&players.at(0))) {
		return;
	}

	st_position scroll_move = checkScrolling();
	loaded_stage->changeScrolling(scroll_move);
	loaded_stage->showStage();
	loaded_stage->move_objects();
	if (can_characters_move == true) {
		players.at(0).execute();
		loaded_stage->move_npcs();
	}
	loaded_stage->show_npcs();
	players.at(0).show();
	loaded_stage->showAbove();
	graphLib.draw_hp_bar(players.at(0).get_current_hp(), players.at(0).get_number(), WEAPON_DEFAULT);
	if (players.at(0).get_selected_weapon() != WEAPON_DEFAULT) {
		graphLib.draw_hp_bar(players.at(0).get_weapon_value(players.at(0).get_selected_weapon()), players.at(0).get_number(), players.at(0).get_selected_weapon());
	}
	//debug_draw_map_links();
	fps_count();
	framerate_limiter();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int game::getMapPointLock(struct st_position pos)
{
	return loaded_stage->getMapPointLock(pos);
}

void game::framerate_limiter()
{
	// FPS control (40 fps is the target)
//#ifndef HANDHELD
	int t2 = timer.getTimer() - _t1; // check the time last frame update took
	if (t2 < _frame_duration) {
		timer.delay(_frame_duration-t2);
	}
//#endif
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_position game::checkScrolling()
{
	st_position move;
	st_position mapScroll = loaded_stage->getMapScrolling();
	st_position p1Pos = players.at(0).getPosition();

	//cout << "game::checkScrolling - p1Pos.x: " << p1Pos.x << ", mapScroll.x: " << mapScroll.x << ", RES_W/2: " << RES_W/2 << "\n";


	move.x += (p1Pos.x - mapScroll.x) - RES_W/2;

	if (mapScroll.x + move.x < 0 || mapScroll.x + move.x > MAP_W*TILESIZE) {
		move.x = 0;
	}

	return move;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::start_stage()
{
	input.clean();
	loaded_stage->reset_current_map();
	// TODO - this must be on a single method in soundlib
	players.at(0).set_position(st_position(80, -TILESIZE));
	soundManager.stop_music();
	soundManager.load_stage_music(currentStage);
	soundManager.play_music();
	loaded_stage->reload_stage(game_object_list);
	players.at(0).clean_projectiles();
	players.at(0).set_animation_type(ANIM_TYPE_TELEPORT);
	players.at(0).set_map(loaded_stage->get_current_map());
	players.at(0).reset_hp();
	loaded_stage->showStage();
	loaded_stage->showAbove();
	graphLib.set_colormap(currentStage);
	graphLib.updateScreen();
	graphLib.show_ready();
	/// @TODO: do not show twice
	if (quick_load == false) {
		while (players.at(0).get_anim_type() == ANIM_TYPE_TELEPORT) {
			showGame();
			graphLib.updateScreen();
		}
		for (int i=0; i<15; i++) { // extra delay to show dialogs
			showGame(false);
			graphLib.updateScreen();
		}
        game_dialogs.show_stage_dialog(currentStage);
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::restart_stage()
{
	input.clean();
	loaded_stage->reset_current_map();
	// TODO - this must be on a single method in soundlib
	players.at(0).set_position(st_position(checkpoint.x, -TILESIZE));
	soundManager.stop_music();
	soundManager.load_stage_music(currentStage);
	soundManager.play_music();
	//loaded_stage->reload_stage(game_object_list);
	players.at(0).clean_projectiles();
	players.at(0).set_animation_type(ANIM_TYPE_TELEPORT);
	players.at(0).set_map(loaded_stage->get_current_map());
	players.at(0).reset_hp();
	loaded_stage->showStage();
	loaded_stage->showAbove();
	graphLib.set_colormap(currentStage);
	graphLib.updateScreen();
	graphLib.show_ready();
}



// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool game::showIntro()
{
    scenes.preloadScenes();
	scenes.intro();
	graphLib.set_colormap(-1);
	scenes.main_screen();
	currentStage = 0;
	initGame();
	if (game_save.stages[INTRO_STAGE] == 0) {
		input.clean();
		start_stage();
	} else {
		selected_stage = scenes.pick_stage();
		currentStage = selected_stage.y * 3 + selected_stage.x + 1;
		loaded_stage = new stage(currentStage, game_object_list, players);
		start_stage();
	}
	std::cout << "game::showIntro - selected_stage.x: " << selected_stage.x << ", selected_stage.y: " << selected_stage.y << ", currentStage: " << currentStage << std::endl;
	return true;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::fps_count()
{
	fps_counter++;
	if (fps_timer <= timer.getTimer()) {
		fps_msg.str(std::string());
		fps_msg << "fps: " << fps_counter;
		fps_counter = 0;
		fps_timer = timer.getTimer()+1000;
	}
	graphLib.draw_text(0, 10, fps_msg.str());
}





// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::load_map_links() {
	for (int i=0; i<MAX_STAGES; i++) {
		if (game_data.stages[i].id != -1) {
			for (int j=0; j<STAGE_MAX_LINKS; j++) {
				if (game_data.stages[i].links[j].id_map_origin != -1 && game_data.stages[i].links[j].id_map_destiny != -1) {
					/// @TODO: instead of converting, we should simply use a list of the new format
					struct format_v1_0::st_map_link link;
					link.stage = i;
					link.is_door = game_data.stages[i].links[j].is_door;
					link.link_bidi = game_data.stages[i].links[j].bidirecional;
					link.link_dest.x = game_data.stages[i].links[j].pos_destiny.x;
					link.link_dest.y = game_data.stages[i].links[j].pos_destiny.y;
					link.link_pos.x = game_data.stages[i].links[j].pos_origin.x;
					link.link_pos.y = game_data.stages[i].links[j].pos_origin.y;
					link.link_size = game_data.stages[i].links[j].size;
					link.map_dest = game_data.stages[i].links[j].id_map_destiny;
					link.map_origin = game_data.stages[i].links[j].id_map_origin;
					link.link_type = game_data.stages[i].links[j].type;
					map_link_list.push_back(link);
					//std::cout << "added link in stage [" << link.stage << "]from map[" << link.map_origin << "][" << link.link_pos.x << ", " << link.link_pos.y << "] to map[" << link.map_dest << "][" << link.link_dest.x << ", " << link.link_dest.y << "]" << std::endl;
				}
			}
		}
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
bool game::test_teleport(classPlayer *test_player) {
	std::vector<struct format_v1_0::st_map_link>::iterator it;
	stage *temp_stage = loaded_stage;
	int currentMap = loaded_stage->get_current_map_number();
	bool must_test;
	int temp_x, temp_y;
	int temp_map_n=0;
	int player_x = 0;
	int player_y = 0;
	int transition_type = 0;
	int lim1, lim2, lim3, lim4;
	int i=1;
	bool MUST_TELEPORT = false;
	int teleporter_dist = 0;
	int teleport_n = -1;
	int link_type = -1;

	int px = test_player->getPosition().x;
	int py = test_player->getPosition().y;
	int pw = test_player->get_size().width;
	int ph = test_player->get_size().heigth;

	//int diff_w = (pw-TILESIZE)*0.5;
	px = px+(pw*0.5);
	py = py+(ph*0.5)+(ph*0.25);


	//std::cout << "game::test_teleport" << std::endl;
	struct format_v1_0::st_map_link temp_link;

	for (it=map_link_list.begin(); it != map_link_list.end(); it++) {
		temp_link = *(it);
		must_test = false;
		if (temp_link.stage == currentStage && temp_link.map_origin != -1) {

			if ((temp_link.map_origin == currentMap && temp_link.link_pos.x != -1)) {
				//std::cout << "game::test_teleport - SAME STAGE 1" << std::endl;
				temp_x = temp_link.link_pos.x;
				temp_y = temp_link.link_pos.y;
				temp_map_n = temp_link.map_dest;
				player_x = temp_link.link_dest.x;
				player_y = temp_link.link_dest.y;
				if (temp_link.link_pos.y > temp_link.link_dest.y) {
					transition_type = TRANSITION_TOP_TO_BOTTOM;
				} else if (temp_link.link_pos.y < temp_link.link_dest.y) {
					transition_type = TRANSITION_BOTTOM_TO_TOP;
				}
				must_test = true;
			} else if (temp_link.map_dest == currentMap && temp_link.link_bidi == 1 && temp_link.link_dest.x != -1) {
				//std::cout << "game::test_teleport - SAME STAGE 2" << std::endl;
				temp_x = temp_link.link_dest.x;
				temp_y = temp_link.link_dest.y;
				temp_map_n = temp_link.map_origin;
				player_x = temp_link.link_pos.x;
				player_y = temp_link.link_pos.y;
				if (temp_link.link_pos.y < temp_link.link_dest.y) {
					transition_type = TRANSITION_TOP_TO_BOTTOM;
				} else if (temp_link.link_pos.y > temp_link.link_dest.y) {
					transition_type = TRANSITION_BOTTOM_TO_TOP;
				}
				must_test = true;
			}

			lim1 = temp_x*TILESIZE;
			lim2 = temp_x*TILESIZE + temp_link.link_size*TILESIZE;
			lim3 = (temp_y)*TILESIZE;
			lim4 = ((temp_y)*TILESIZE)+TILESIZE;

			//printf(">>>>>>>>>>> test_map_teleport, lim1: %d, lim2: %d, px: %d\n", lim1, lim2, px);
			//printf(">>>>>>>>>>> test_map_teleport, lim3: %d, lim4: %d, py: %d\n", lim3, lim4, py);

			if ((px >= lim1 && px <= lim2) && ((py > lim3 && py < lim4))) {
				//printf("****> test_map_teleport: i: %d, origin: %d, destiny: %d, size: %d\n", i, temp_link.map_origin, temp_link.map_dest, temp_link.link_size);
				if (test_player->get_teleporter() == -1) {
					//printf(">>>>>>>>>>> test_map_teleport - MUST TELEPORT i: %d, lim1: %d, lim2: %d, px: %d\n", i, lim1, lim2, px);
					//printf("**** test_map_teleport::MUST TELEPORT - origin: %d, destiny: %d, size: %d, type: %d\n", temp_link.map_origin, temp_link.map_dest, temp_link.link_size, temp_link.link_type);
					teleporter_dist = lim1 - player_x*TILESIZE - 8;
					MUST_TELEPORT = true;
					teleport_n = i;
					//std::cout << ">> test_map_teleport.set_teleport(" << i << ") <<" << std::endl;
					test_player->set_teleporter(i);
					link_type = temp_link.link_type;
					break;
				}
			// only clean teleport when player is out of the teleporter
			} else {
				//printf(">> testing out of teleporter[%d] for player->teleporter[%d] <<\n", i, playerObj->teleporter);
				if (i == test_player->get_teleporter()) {
					//std::cout << ">> test_map_teleport.clean_teleport <<" << std::endl;
					test_player->set_teleporter(-1);
				}
			}


		}
		i++;
	}
	if (MUST_TELEPORT) {
		remove_all_projectiles();
		// --------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>
		// TODO - define according to the condition, try to maintain it
		//playerObj->sprite->anim_type = ANIM_STAIRS;
		// --------------------->>>>>>>>>>>>>>>>>>>>>>>>>>>>
		int dest_x = (player_x*TILESIZE) - temp_stage->getMapScrolling().x;


		int max_pos_x = MAP_W*TILESIZE - RES_W;
		if (dest_x > max_pos_x) {
		   dest_x = max_pos_x;
		}
		if (dest_x < 0) {
		   dest_x = 0;
		}

		//std::cout << ">>>>>>>>>>>> TELEPORT - temp_map_n: " << temp_map_n << ", dest_x: " << dest_x << " <<<<<<<<<<<<" << std::endl;


		// must move the map, so that the dest position in screen is equal to player_real_pos_x
		int new_map_pos_x;
		int playerRealXPos = test_player->getPosition().x + temp_stage->getMapScrolling().x;

		//new_map_pos_x = ((player_x*TILESIZE)) - playerRealXPos + teleporter_dist;
		//std::cout << "game::test_teleport - playerRealXPos: " << playerRealXPos << ", teleporter_dist: " << teleporter_dist << ", link.dest.x: " << player_x << std::endl;
		new_map_pos_x = temp_stage->getMapScrolling().x - teleporter_dist;
		if (new_map_pos_x < 0) {
		   new_map_pos_x = 0;
		} else if (new_map_pos_x > MAP_W*TILESIZE) {
		   new_map_pos_x = MAP_W*TILESIZE;
		}

		int diff_h=6;

		if (test_player->get_size().width > 30) {
		   diff_h = abs(test_player->get_size().width-30);
		}
		new_map_pos_x -= diff_h +2;

		//std::cout << "******************* game::test_teleport - teleporter_dist: " << teleporter_dist << ", new_map_pos_x: " << new_map_pos_x << std::endl;


		if (link_type == LINK_TELEPORTER) {
		   test_player->teleport_stand();
		   graphLib.blank_screen();
		   graphLib.updateScreen();
		   input.waitTime(500);
		   currentMap = temp_map_n;
		   new_map_pos_x = (temp_link.link_dest.x * TILESIZE) - TILESIZE*2;
		   //test_player->set_position(st_position(temp_link.link_dest.x * TILESIZE, temp_link.link_dest.y * TILESIZE));
		} else {
		   transitionScreen(transition_type, temp_map_n, new_map_pos_x, test_player);
		   //test_player->set_position(st_position(playerRealXPos+new_map_pos_x, test_player->getPosition().y));

		}


		set_current_map(temp_map_n);
		loaded_stage->set_scrolling(st_position(new_map_pos_x, temp_stage->getMapScrolling().y));
		st_position new_map_scroll = loaded_stage->get_current_map()->getMapScrolling();
		//std::cout << "////////////// GAME - new_map_scroll.x: " << new_map_scroll.x << std::endl;

		// must compensate the diference of scrolling in the player x position
		//std::cout << ">>>>>>>>>> character::set_position - real.old.x: " << test_player->get_real_position().x << ", new.x: " << (test_player->getPosition().x-teleporter_dist-diff_h-2) << std::endl;

		//test_player->set_position(st_position(test_player->getPosition().x-teleporter_dist-diff_h-2, test_player->getPosition().y));
		test_player->set_position(st_position(abs(test_player->get_real_position().x+new_map_pos_x), test_player->getPosition().y));
		test_player->char_update_real_position();


		loaded_stage->get_current_map()->reset_scrolled();

		graphLib.updateScreen();
		//input.waitTime(200);

		/*

		if (k == 0) {
		   pObj2 = getPlayerN(1);
		} else {
		   pObj2 = getPlayerN(0);
		}
		if (link_type != LINK_TELEPORTER) {
		   pObj2->sprite->x = playerRealXPos + new_map_pos_x;
		} else {
		   pObj2->sprite->x = new_map_pos_x+ TILE_SIZE * 2;
		}
		playerObj->teleporter = i;
		pObj2->teleporter = i;
		return 1;

		*/
		return true;

	}
	return false;
}

void game::set_current_map(int temp_map_n)
{
	//std::cout << "game::set_current_map - temp_map_n: " << temp_map_n << std::endl;
	loaded_stage->set_current_map(temp_map_n);
	players.at(0).set_map(loaded_stage->get_current_map());

	if (loaded_stage->get_current_map() != players.at(0).map) {
		std::cout << ">>>>>>>>>>>>>> ERROR" << std::endl;
		exit(-1);
	}

/*
loaded_stage->get_current_map() != players.at(0).map
*/
}

short game::get_current_map()
{
	return loaded_stage->get_current_map_n();;
}

void game::boss_intro()
{
	//std::cout << "SHOW BOSS INTRO" << std::endl;
	// 1. keep showing game screen until player reaches ground
	players.at(0).clear_move_commands();
	for (int i=0; i<20; i++) {
		loaded_stage->showStage();
		players.at(0).charMove();
		players.at(0).show();
		loaded_stage->showAbove();
		graphLib.draw_hp_bar(players.at(0).get_current_hp(), players.at(0).get_number(), WEAPON_DEFAULT);
		input.waitTime(8);
		graphLib.updateScreen();
	}

	// 2. blink screen
	graphLib.blink_screen(255, 255, 255);

	// 3. move boss from top to ground
	for (int i=0; i<230; i++) {
		loaded_stage->showStage();
		loaded_stage->move_npcs();
		loaded_stage->show_npcs();
		players.at(0).show();
		loaded_stage->showAbove();
		graphLib.draw_hp_bar(players.at(0).get_current_hp(), players.at(0).get_number(), WEAPON_DEFAULT);
		input.waitTime(8);
		graphLib.updateScreen();
	}


	// 4. show boss dialog
	if (strlen(game_data.stages[currentStage].boss_dialog.line1[players.at(0).get_number()]) > 0) {
		dialogs boss_dialog;
		boss_dialog.show_boss_dialog(currentStage);
	}
}

object* game::get_player_platform()
{
	players.at(0).get_platform();
}


// ********************************************************************************************** //
// ********************************************************************************************** //
// method used for debug, draws the teleporter numbers in map (like as in the editor)
void game::debug_draw_map_links()
{
	std::vector<struct format_v1_0::st_map_link>::iterator it;
	st_position mapScroll = loaded_stage->getMapScrolling();
	int i=0;
	stage *temp_stage = loaded_stage;
	int currentMap = temp_stage->get_current_map_number();
	std::stringstream text_i;

	for (it=map_link_list.begin(); it != map_link_list.end(); it++) {
		struct format_v1_0::st_map_link temp_link = *(it);
		if (temp_link.stage == currentStage && temp_link.map_origin != -1) {
			// outgoind link
			if ((temp_link.map_origin == currentMap && temp_link.link_pos.x != -1)) {
				text_i.str(std::string());
				text_i << i;
				graphLib.draw_text(temp_link.link_pos.x*TILESIZE-mapScroll.x, temp_link.link_pos.y*TILESIZE+mapScroll.y, text_i.str());
				i++;
			// incoming link that is bidirecional
			} else if (temp_link.map_dest == currentMap && temp_link.link_bidi == 1 && temp_link.link_dest.x != -1) {
				text_i.str(std::string());
				text_i << i;
				i++;
				graphLib.draw_text(temp_link.link_dest.x*TILESIZE-mapScroll.x, temp_link.link_dest.y*TILESIZE+mapScroll.y, text_i.str());
			}
		}
	}
}




// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void game::load_game_objects()
{
//#define LOAD_OBJS_MODE 1

#ifdef LOAD_OBJS_MODE
	std::string filename;
	ifstream fp;
	//object new_obj;
	struct format_v1_0::st_object temp_obj;

	for (int i=1; i<999; i++) {
		filename = FILEPATH + "data/game/game_001/" + cent_number(i) + ".obj";
		fp.open(filename.c_str(), ios::out | ios::binary | ios::app);

		if (!fp.is_open()) {
			cout << "ERROR::game::load_game_objects - could not load file '" + filename + "'\n";
			return;
		}

		fp.read(reinterpret_cast<char *>(&temp_obj), sizeof(temp_obj));
		object new_obj(i, temp_obj);
		fp.close();
		game_object_list.insert(std::pair<std::string, object>(temp_obj.id, new_obj));
		//std::cout << "game::load_game_objects - added object with id '" << temp_obj.id << "' in game_object_list." << std::endl;
		//game_object_list.push_back(new_obj);
	}
#else
	for (int i=0; i<GAME_MAX_OBJS; i++) {
		if (game_data.objects[i].id != -1) {
			object new_obj(i, game_data.objects[i]);
			//std::cout << "npc[" << i << "].name[" << game_data.objects[i].name << "].id: " << game_data.objects[i].id << std::endl;
			game_object_list.insert(std::pair<std::string, object>(cent_number(game_data.objects[i].id), new_obj));
		}
	}
#endif
}



std::string game::cent_number(int n) {
	std::stringstream ss;

	if (n < 10) {
		ss << "00" << n;
	} else if (n < 100) {
		ss << "0" << n;
	} else {
		ss << n;
	}
	return ss.str();
}


// ********************************************************************************************** //
// remove the projectiles from the list of all players and npcs                                   //
// ********************************************************************************************** //
void game::remove_all_projectiles()
{
	std::vector<classPlayer>::iterator p_it;
	for (p_it=players.begin(); p_it!=players.end(); p_it++) {
		(*p_it).clean_projectiles();
	}
	stage *temp_stage = loaded_stage;
	temp_stage->get_current_map()->clean_map_npcs_projectiles();
}


//TRANSITION_TOP_TO_BOTTOM, TRANSITION_BOTTOM_TO_TOP
void game::transitionScreen(short int type, short int map_n, short int adjust_x, classPlayer *pObj) {
	graphicsLib_gSurface temp_screen;
	short int i;
	stage* temp_stage = loaded_stage;
	graphLib.initSurface(st_size(RES_W, RES_H*2), &temp_screen);
	classMap* temp_map = temp_stage->maps[map_n];
	// draw map in the screen, erasing all players/objects/npcs
	loaded_stage->showStage();

	//std::cout << "transitionScreen::START - p.x: " << pObj->getPosition().x << ", p.y: " << pObj->getPosition().y << std::endl;

	// draw the offscreen with the new loaded map

	if (type == TRANSITION_TOP_TO_BOTTOM || type == TRANSITION_BOTTOM_TO_TOP) {
		// copy current screen to temp
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			graphLib.copyArea(st_rectangle(0, 0, RES_W, RES_H), st_position(0, 0), &graphLib.gameScreen, &temp_screen);
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			graphLib.copyArea(st_rectangle(0, 0, RES_W, RES_H), st_position(0, RES_H), &graphLib.gameScreen, &temp_screen);
		}
		// copy the new screen to the temp_area
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			graphLib.copyArea(st_rectangle(adjust_x, TILESIZE, RES_W, RES_H), st_position(0, RES_H), temp_map->get_map_surface(), &temp_screen);
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			graphLib.copyArea(st_rectangle(adjust_x, TILESIZE, RES_W, RES_H), st_position(0, 0), temp_map->get_map_surface(), &temp_screen);
		}


		// now, show the transition
		for (i=0; i<(RES_H+TILESIZE*0.5)/TRANSITION_STEP; i++) {
			//exec_colorcycle(temp_screen);
			if (type == TRANSITION_TOP_TO_BOTTOM) {
				graphLib.copyArea(st_rectangle(0, i*TRANSITION_STEP, RES_W, RES_H), st_position(0, 0), &temp_screen, &graphLib.gameScreen);
			} else if (type == TRANSITION_BOTTOM_TO_TOP) {
				graphLib.copyArea(st_rectangle(0, RES_H-i*TRANSITION_STEP, RES_W, RES_H), st_position(0, 0), &temp_screen, &graphLib.gameScreen);
			}

			if (type == TRANSITION_TOP_TO_BOTTOM) {
				if (pObj->getPosition().y > TILESIZE) {
					pObj->set_position(st_position(pObj->getPosition().x, pObj->getPosition().y - TRANSITION_STEP));
				}
			} else if (type == TRANSITION_BOTTOM_TO_TOP) {
				if (pObj->getPosition().y < RES_H-TILESIZE*2) {
					pObj->set_position(st_position(pObj->getPosition().x, pObj->getPosition().y + TRANSITION_STEP));
				}
			}
			pObj->char_update_real_position();
			pObj->show();
			loaded_stage->showAbove();
			graphLib.updateScreen();
			input.waitTime(6);
		}
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			temp_map->changeScrolling(st_position(temp_map->getMapScrolling().x, 0));
		}
		if (type == TRANSITION_TOP_TO_BOTTOM) {
			if (pObj->getPosition().y > TILESIZE) {
				pObj->set_position(st_position(pObj->getPosition().x, pObj->getPosition().y - TRANSITION_STEP));
			}
		} else if (type == TRANSITION_BOTTOM_TO_TOP) {
			if (pObj->getPosition().y < RES_H-TILESIZE*2) {
				pObj->set_position(st_position(pObj->getPosition().x, pObj->getPosition().y + TRANSITION_STEP));
			}
		}
	}
	pObj->char_update_real_position();

	//std::cout << "transitionScreen::END - p.x: " << pObj->getPosition().x << ", p.y: " << pObj->getPosition().y << std::endl;

}

/*
void game::horizontal_transition_screen(int tileX, int tileY, bool is_door, character* playerObj) {

	int i, multi, upTile, downTile;
	bool is_on_left = false;
	classMap* temp_map = loaded_stage->get_current_map();
	st_position p_pos = playerObj->getPosition();

	std::cout << "game::horizontal_transition_screen" << std::endl;

	//printf(">>>> PLAYER HIT THE BOSS DOOR, will move %d\n", RES_W);
	multi = (RES_W)/(TILESIZE+playerObj->get_size().width);

	//printf(">>>> PLAYER IS ON LEFT OF BOSS DOOR\n");
	if (p_pos.x < tileX*TILESIZE-TILESIZE*0.5) {
		is_on_left = true;
	}
	if (is_door == true) {
		loaded_stage->showStage();
		upTile = tileY;
		for (i=tileY; i>=0; i--) {
			if (temp_map->getMapPointLock(st_position(tileX, i)) == TERRAIN_DOOR) {
				upTile = i;
			} else {
				break;
			}
		}
		downTile = tileY;
		for (i=tileY; i<MAP_H; i++) {
			if (temp_map->getMapPointLock(st_position(tileX, i)) == TERRAIN_DOOR) {
				downTile = i;
			} else {
				break;
			}
		}
		soundManager.play_sfx(SFX_DOOR_OPEN);
		loaded_stage->redraw_boss_door(false, (downTile-upTile+1), tileX, players.at(0).get_number());//bool is_close, int nTiles, int tileX
	}


	//int player_transiction_step = 20/((RES_W)/DOOR_MOVE_STEP);
	for (i=0; i<(RES_W-TILESIZE)/DOOR_MOVE_STEP; i++) {
		if (is_on_left) {
			temp_map->move_map(DOOR_MOVE_STEP, 0);
			if (i%multi == 0) {
				//std::cout << "game::horizontal_transition_screen - p.x++" << std::endl;
				playerObj->set_position(st_position(playerObj->getPosition().x+2, p_pos.y));
			}
		} else {
			temp_map->move_map(-DOOR_MOVE_STEP, 0);
			if (i%multi == 0) {
				//std::cout << "game::horizontal_transition_screen - p.x--" << std::endl;
				playerObj->set_position(st_position(playerObj->getPosition().x-2, p_pos.y));
			}
		}
		loaded_stage->showStage();
		playerObj->char_update_real_position();
		playerObj->show();
		if (is_door == false) {
			loaded_stage->showAbove();
		}
		std::cout << "********* loop - p.x: " << playerObj->getPosition().x << ", p-real.x: " << playerObj->get_real_position().x << std::endl;
		graphLib.draw_hp_bar(playerObj->get_current_hp(), players.at(0).get_number(), WEAPON_DEFAULT);
		graphLib.updateScreen();
		input.waitTime(6);
	}

	if (is_door == true) {
		soundManager.play_sfx(SFX_DOOR_OPEN);
		loaded_stage->redraw_boss_door(true, (downTile-upTile+1), tileX, players.at(0).get_number());
	}
	std::cout << ">>>>>>>>> end - p.x: " << playerObj->getPosition().x << ", p-real.x: " << playerObj->get_real_position().x << std::endl;
}
*/

void game::horizontal_screen_move(short direction, bool is_door, short tileX, short tileY)
{
	int i, upTile, downTile;
	classMap* temp_map = loaded_stage->get_current_map();
	st_position scroll_move;
	if (direction == ANIM_DIRECTION_LEFT) {
		scroll_move.x = -2;
	} else {
		scroll_move.x = 2;
	}


	if (is_door == true) {
		loaded_stage->showStage();
		upTile = tileY;
		for (i=tileY; i>=0; i--) {
			if (temp_map->getMapPointLock(st_position(tileX, i)) == TERRAIN_DOOR) {
				upTile = i;
			} else {
				break;
			}
		}
		downTile = tileY;
		for (i=tileY; i<MAP_H; i++) {
			if (temp_map->getMapPointLock(st_position(tileX, i)) == TERRAIN_DOOR) {
				downTile = i;
			} else {
				break;
			}
		}
		soundManager.play_sfx(SFX_DOOR_OPEN);
		loaded_stage->redraw_boss_door(false, (downTile-upTile+1), tileX, players.at(0).get_number());//bool is_close, int nTiles, int tileX
	}


	int move_limit = (RES_W/abs(scroll_move.x))-TILESIZE/abs(scroll_move.x);
	//int move_limit = (RES_W/abs(scroll_move.x));
	for (int i=0; i<move_limit; i++) {
		loaded_stage->changeScrolling(scroll_move, false);
		loaded_stage->showStage();
		loaded_stage->show_npcs();
		players.at(0).show();
		if (is_door == false) { /// @TODO - pass a parameter to showAbove not to draw door in a given tile_x
			loaded_stage->showAbove();
		}
		graphLib.draw_hp_bar(players.at(0).get_current_hp(), players.at(0).get_number(), WEAPON_DEFAULT);
		input.waitTime(8);
		graphLib.updateScreen();
		if (i%(TILESIZE/2) == 0) {
			players.at(0).set_position(st_position(players.at(0).getPosition().x+scroll_move.x, players.at(0).getPosition().y));
		}
	}
	if (is_door == true) {
		soundManager.play_sfx(SFX_DOOR_OPEN);
		loaded_stage->redraw_boss_door(true, (downTile-upTile+1), tileX, players.at(0).get_number());
	}
	//std::cout << "horizontal_screen_move END" << std::endl;
	loaded_stage->showStage();
}

void game::got_weapon(std::string npc_name)
{
	if (game_save.stages[currentStage] == 0) {
		game_save.finished_stages++;
		game_save.stages[currentStage] = 1;
		format_v_2_0_1::file_io fio;
		fio.write_save(game_save);
	}
	//std::cout << "game::got_weapon - npc_name: " << npc_name << std::endl;

	if (currentStage == SKULLCASTLE1 && get_current_map() == 2 && npc_name == "Destrin Tank") {
		loaded_stage->set_scrolling(st_position(0, 0));
		ending game_ending;
		game_ending.start();
		exit(-1);
	}

	if ((currentStage != 0 && currentStage <= 8)) {
		// check witch is the boss that was killed
		//if (npc_name ==)

		/// @TODO: save game

		/// @TODO: teletransport if capsules

		/// @TODO: walk to boss room middle, if inside a boss room (must create a way to check this), otherwise, just jump up

		// jump up, implosion, jump down and teleport out of the screen
		players.at(0).execute_jump_up();
		draw_implosion(players.at(0).get_real_position().x+players.at(0).get_size().width/2, players.at(0).get_real_position().y+players.at(0).get_size().heigth/2);
		players.at(0).set_weapon(WEAPON_APEBOT); /// @TODO
		players.at(0).fall();
		soundManager.play_sfx(SFX_GOT_WEAPON);
		input.waitTime(5000);
		soundManager.play_sfx(SFX_TELEPORT);
		players.at(0).teleport_out();

		/// @TODO
		// show the "you got" screen
		graphLib.blank_screen();
		graphLib.blink_screen(255, 255, 255);
		graphLib.blank_screen();
		graphicsLib_gSurface temp_bg;
		graphLib.surfaceFromFile(FILEPATH+"data/images/backgrounds/stage_boss_intro.png", &temp_bg);
		graphLib.showSurface(&temp_bg);

		players.at(0).set_position(st_position(20, 88));
		players.at(0).set_animation_type(ANIM_TYPE_ATTACK);
		loaded_stage->set_scrolling(st_position(0, 0));
		players.at(0).char_update_real_position();
		players.at(0).show();

		graphLib.init_stars();

		std::string weapon_name(game_data.weapons[game_data.stages[currentStage].boss.id_weapon].name);
		for (std::string::iterator p = weapon_name.begin(); weapon_name.end() != p; ++p) {
			*p = toupper(*p);
		}

		std::string phrase = std::string("YOU GOT ") + weapon_name;

		short int pos_text_x = RES_W*0.5 - 90;
		graphLib.draw_progressive_text(pos_text_x, 90, phrase, false, true);
		/*
		if (STAGE_N == 1) {
			sprintf(extra_name, "AND FROG COIL ADAPTOR");
		} else if (STAGE_N == 6) {
			sprintf(extra_name, "AND EAGLE JET ADAPTOR");
		} else {
			sprintf(extra_name, " ");
		}
		*/

		players.at(0).show();

		unsigned int timeInit = timer.getTimer() + 2000;
		while (timeInit > timer.getTimer()) {
			graphLib.waitTimeWithStars(20);
		}
	} else {
		players.at(0).teleport_out();
	}



	// return to stage selection
	players.at(0).set_weapon(WEAPON_DEFAULT);
	loaded_stage->clean_stage();
	delete loaded_stage;
	selected_stage = scenes.pick_stage();
	currentStage = selected_stage.y * 3 + selected_stage.x + 1;
	loaded_stage = new stage(currentStage, game_object_list, players);
	checkpoint.map = 0;
	checkpoint.map_scroll_x = 0;
	checkpoint.x = 50;
	start_stage();
}

void game::quick_load_game()
{
	format_v_2_0_1::file_io fio;
	if (fio.save_exists()) {
		fio.read_save(game_save);
	}
	scenes.preloadScenes();
	initGame();
	start_stage();
}

void game::draw_explosion(short int centerX, short int centerY, bool show_players) {
	unsigned int timerInit;
	int distance=0, mode=0;
	graphicsLib_gSurface tempImg;
	int accel=1;

	timerInit = timer.getTimer();

	std::string filename = FILEPATH + "data/images/tilesets/explosion_boss.png";
	graphLib.surfaceFromFile(filename, &tempImg);

	if (show_players) {
		players.at(0).set_animation_type(ANIM_TYPE_STAND);
	}



	//p1Obj->sprite->anim_type = ANIM_STAND;
	graphLib.updateScreen();
	soundManager.stop_music();
	soundManager.play_sfx(SFX_PLAYER_DEATH);
	// x = a + r * cos(t)
	// y = b + r * sin(t)
	while (timer.getTimer() < timerInit+2000) {
		loaded_stage->showStage();
		loaded_stage->showAbove();
		if (show_players) {
			players.at(0).show();
		}
		for (int i=0; i<6; i++) {
			graphLib.copyArea(st_rectangle(TILESIZE*mode, 0, tempImg.width/3, tempImg.height), st_position(centerX+distance*cos(static_cast<double>(i*45)), centerY+distance*sin(static_cast<double>(i*45))), &tempImg, &graphLib.gameScreen);
		}
		if (distance > 50) {
			for (int i=0; i<6; i++) {
				graphLib.copyArea(st_rectangle(TILESIZE*mode, 0, tempImg.width/3, tempImg.height), st_position(centerX+(distance-50)*cos(static_cast<double>(i*45)), centerY+(distance-50)*sin(static_cast<double>(i*45))), &tempImg, &graphLib.gameScreen);
			}
		}
		distance += 3;
		if (distance % 6 == 0) {
			if (mode+1 <= 2) {
				mode++;
			} else {
				mode = 0;
			}
			if (accel +1 <= 5) {
				accel++;
			}
		}
		graphLib.updateScreen();
		input.waitTime(10);
	}
}

void game::draw_implosion(short int centerX, short int centerY) {
	unsigned int timerInit;
	int distance=RES_W*0.5, mode=0;
	graphicsLib_gSurface tempImg;
	int accel=1;
    int second_distance = 100;

	timerInit = timer.getTimer();

	std::string filename = FILEPATH + "data/images/tilesets/explosion_boss.png";
	graphLib.surfaceFromFile(filename, &tempImg);

	graphLib.updateScreen();
	soundManager.stop_music();
    soundManager.play_repeated_sfx(SFX_IMPLOSION, 1);
	// x = a + r * cos(t)
	// y = b + r * sin(t)
    while (distance > -second_distance) {
		//std::cout << "distance: " << distance << std::endl;
		loaded_stage->showStage();
		players.at(0).show();
		loaded_stage->showAbove();
		//loaded_stage->showAbove();
        if (distance > 0) {
            for (int i=0; i<6; i++) {
				graphLib.copyArea(st_rectangle(TILESIZE*mode, 0, tempImg.width/3, tempImg.height), st_position(centerX+distance*cos(static_cast<double>(i*45)), centerY+distance*sin(static_cast<double>(i*45))), &tempImg, &graphLib.gameScreen);
            }
        }
		if (distance < RES_W*0.5-50) {
            for (int i=0; i<6; i++) {
				graphLib.copyArea(st_rectangle(TILESIZE*mode, 0, tempImg.width/3, tempImg.height), st_position(centerX+(distance+second_distance)*cos(static_cast<double>(i*45)), centerY+(distance+second_distance)*sin(static_cast<double>(i*45))), &tempImg, &graphLib.gameScreen);
			}
		}
		distance -= 3;
		if (abs(distance) % 4 == 0) {
			if (mode+1 <= 2) {
				mode++;
			} else {
				mode = 0;
			}
			if (accel +1 <= 5) {
				accel++;
			}
		}
		graphLib.updateScreen();
        input.waitTime(15);
	}
/*
	unsigned int timerInit;
	char tempBuffer[100];
	int distance=RES_W*0.5, mode=0;
	SDL_Surface *tempImg;
	int accel=1;

	play_timed_sfx(SFX_IMPLOSION, 1200);
	waitTime(200);

	timerInit = timer.getTimer();

	sprintf(tempBuffer, "%sdata/images/tilesets/explosion_boss.png", FILEPATH);
	tempImg = surfaceFromFile(tempBuffer);
	SDL_SetColors(tempImg, colormap, 0, COLOR_COUNT);
	updateScreen(game_screen);
	centerX -= tempImg->w/6;
	centerY -= tempImg->h/2;

	while (distance > 0) {
		move_screen(game_screen, 0, 0);
		show_sprite(p1Obj->sprite, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY+distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX+distance, centerY-distance, game_screen);
		copy_area(tempImg, TILE_SIZE*mode, 0, tempImg->w/3, tempImg->h, centerX-distance, centerY-distance, game_screen);
		distance -= 3;
		if (abs(distance) % 4 == 0) {
			if (mode+1 <= 2) {
				mode++;
			} else {
				mode = 0;
			}
			if (accel +1 <= 5) {
				accel++;
			}
		}
		drawMap3rdLevel(game_screen);
		updateScreen(game_screen);
		waitTime(5);
	}
	SDL_FreeSurface(tempImg);
*/
}

void game::show_player(short player_n)
{
	players.at(player_n).show();
}

void game::set_player_position(st_position pos, short player_n)
{
	players.at(player_n).set_position(pos);
	players.at(player_n).char_update_real_position();
}

void game::change_player_position(short xinc, short yinc, short player_n)
{
	players.at(player_n).change_position(xinc, yinc);
	players.at(player_n).char_update_real_position();
}

void game::set_player_anim_type(short anim_type, short int player_n)
{
	players.at(player_n).set_anim_type(anim_type);
}

st_position game::get_player_position(short player_n)
{
	return players.at(player_n).getPosition();
}

st_size game::get_player_size(short int player_n)
{
	return players.at(player_n).get_size();
}

void game::set_player_direction(short direction, short player_n)
{
	players.at(player_n).set_direction(direction);
}

void game::player_fall(short int player_n)
{
	players.at(player_n).fall();
}

