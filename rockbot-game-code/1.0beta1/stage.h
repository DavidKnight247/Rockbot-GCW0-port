#ifndef STAGE_H
#define STAGE_H

#include "classmap.h"
#include "character.h"
#include "classplayer.h"

#define PRELOAD_MAP_N 3

class stage
{
public:
	stage(int, std::map<std::string, object> &game_object_list, std::vector<classPlayer> &player_list);
	~stage();
	void setNumber(int);
	void loadStage(std::map<std::string, object> &game_object_list);
	void showStage();
	int getMapPointLock(struct st_position);
	void changeScrolling(st_position pos, bool check_lock=true);
	st_position getMapScrolling();
	void showAbove();
	classMap* get_current_map();
	int get_current_map_number();
	void set_current_map(int);
	short int get_current_map_n();
	void set_scrolling(st_position pos);
	void move_map(const short int move_x, const short int move_y);
	void reload_stage(std::map<std::string, object> &game_object_list); // reload maps npc and object list
	void reset_current_map();
	void clean_stage();
	void redraw_boss_door(bool is_close, int nTiles, int tileX, short player_number);
	void move_npcs();
	void show_npcs();
	void move_objects();

public:
	classMap* maps[PRELOAD_MAP_N];

private:
	int number;
	int currentMap;
	std::vector<character*> _player_list;
};

#endif // STAGE_H
