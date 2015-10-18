#include <iostream>
using namespace std;

#include "stage.h"

extern format_v_2_0_1::file_game game_data;
extern struct format_v_2_0_1::st_checkpoint checkpoint;

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
stage::stage(int setStageN, std::map<std::string, object> &game_object_list, std::vector<classPlayer> &set_player_list)
{
	number = -1;
	currentMap = 0;
	setNumber(setStageN);
	//loadStage(game_object_list);
	for (int i=0; i<PRELOAD_MAP_N; i++) {
		maps[i] = NULL;
	}

	std::vector<classPlayer>::iterator it;
	for (it=set_player_list.begin(); it != set_player_list.end(); it++) {
		classPlayer* temp = &(*it);
		_player_list.push_back(dynamic_cast<character*>(temp));
	}
	//_player_list = player_list;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
stage::~stage()
{
	for (int i=0; i<PRELOAD_MAP_N; i++) {
		if (maps[i] != NULL) {
			delete maps[i];
		}
	}
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void stage::setNumber(int setNumber)
{
	number = setNumber;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void stage::loadStage(std::map<std::string, object> &game_object_list) {
	//std::cout << "*************** stage::loadStage(" << number << ") ***************" << std::endl;
	if (number == -1) {
		cout << "ERROR::stage::loadStage - stage number was not set, can't load it before setting the number.\n";
		return;
	}
	if (number >= MAX_STAGES) {
		std::cout << "ERROR::stage::loadStage - stage number[" << number << "] is greater than the MAX_STAGES[" << MAX_STAGES << "].\n";
		return;
	}

	//std::cout << "stage::loadStage - number: " << number << std::endl;

	// load stage maps
	for (int i=0; i<STAGE_MAX_MAPS; i++) {
		if (game_data.stages[i].id != -1) {
			classMap *new_map = new classMap();
			new_map->setMapNumber(i);
			new_map->setStageNumber(number);
			new_map->loadMap(game_object_list);
			new_map->set_player_list(_player_list);
			maps[i] = new_map;
		}
	}

}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void stage::showStage()
{
	maps[currentMap]->showMap();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void stage::showAbove()
{
	maps[currentMap]->showAbove();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int stage::getMapPointLock(st_position pos)
{
	return maps[currentMap]->getMapPointLock(pos);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void stage::changeScrolling(st_position pos, bool check_lock)
{
	maps[currentMap]->changeScrolling(pos, check_lock);
}

void stage::set_scrolling(st_position pos)
{
	maps[currentMap]->set_scrolling(pos);
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_position stage::getMapScrolling()
{
	return maps[currentMap]->getMapScrolling();
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
classMap* stage::get_current_map()
{
	//std::cout << "**** stage::get_current_map - number: " << maps[currentMap]->number << std::endl;
	return maps[currentMap];
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int stage::get_current_map_number()
{
	return currentMap;
}

void stage::set_current_map(int new_map_n)
{
	//std::cout << "------- stage::set_current_map to " << new_map_n << "-------" << std::endl;
	currentMap = new_map_n;
}

short int stage::get_current_map_n()
{
	return currentMap;
}

void stage::move_map(const short int move_x, const short int move_y)
{
	maps[currentMap]->move_map(move_x, move_y);
}

void stage::reload_stage(std::map<std::string, object> &game_object_list)
{
	std::cout << ">>>>>>>>>>> stage::reload_stage stage(" << number << ") <<<<<<<<<<" << std::endl;
	for (int i=0; i<PRELOAD_MAP_N; i++) {
		delete maps[i];
	}
	loadStage(game_object_list);
}

void stage::reset_current_map()
{
	//std::cout << "================== stage::reset_current_map" << std::endl;
	currentMap = checkpoint.map;
	if (maps[currentMap] != NULL) {
		maps[currentMap]->set_scrolling(st_position(checkpoint.map_scroll_x, 0));
	}
}


void stage::clean_stage()
{
	maps[currentMap]->clean_map();
}

void stage::redraw_boss_door(bool is_close, int nTiles, int tileX, short player_number)
{
	//std::cout << "stage::redraw_boss_door - is_close: " << is_close << std::endl;
	maps[currentMap]->redraw_boss_door(is_close, nTiles, tileX, player_number);
}

void stage::move_npcs()
{
	maps[currentMap]->move_npcs();
}

void stage::show_npcs()
{
	maps[currentMap]->show_npcs();
}

void stage::move_objects()
{
	maps[currentMap]->move_objects();
}

