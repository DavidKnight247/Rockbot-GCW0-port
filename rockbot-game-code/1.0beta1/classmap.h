#ifndef CLASSMAP_H
#define CLASSMAP_H


#include "defines.h"
#include "st_common.h"
#include "graphicslib.h"
#include "classnpc.h"
#include "objects/object.h"
#include "graphic/animation.h"
#include <list>
#include <vector>

#define EXPLOSION_ANIMATION_TIME 50

class character;

struct object_colision {
	int _block;
	object* _object;
	object_colision(int set_block, object* set_object) {
		_block = set_block;
		_object = set_object;
	}
};


class classMap
{
public:
	classMap();
	~classMap();
	static bool deleteAll( classMap* theElement );
	void setStageNumber(int);
	short get_stage_number();
	short get_number();
	void setMapNumber(int);
	void loadMap(std::map<std::string, object> &game_object_list);
	void drawMap();
	void showMap();
	void showAbove();
	int getMapPointLock(st_position pos);
	bool get_map_point_wall_lock(int x);
	void changeScrolling(st_position pos, bool check_lock=true);
    st_position getMapScrolling();
	std::string cent_number(int n);
	st_position get_last_scrolled();
	void reset_scrolled();
	void load_map_objects(std::map<std::string, object> &game_object_list);
	object_colision colision_player_object(character*, const short int, const short int, short int, short int);
	short int colision_player_npcs(character*, const short int, const short int, short int, short int);
	void clean_map_npcs_projectiles();
	graphicsLib_gSurface* get_map_surface();
	void set_scrolling(st_position pos);
	void move_map(const short int move_x, const short int move_y);
	void clean_map();
	void redraw_boss_door(bool is_close, int nTiles, int tileX, short player_number);
    void add_animation(ANIMATION_TYPES pos_type, std::string filename, const st_position &pos, st_position adjust_pos, unsigned int frame_time, unsigned int repeat_times, int direction, st_size framesize);
	void set_player_list(std::vector<character*> player_list);
	void spawn_map_npc(short int npc_id, st_position npc_pos, short direction);
	void move_npcs();
	void show_npcs();
	void move_objects();

private:
	void load_map_npcs();
	void draw_dynamic_backgrounds();

public:
	struct format_v1_0::file_map_v1 map_tiles;
	std::list<classnpc*> npc_list; // vector npcs
	std::vector<character*> _player_list; // vector players
    std::list<animation> animation_list;
	// vector teleporters
	// vector objects
	int number;

private:
	int stage_number;
	graphicsLib_gSurface mapSurface;
	struct st_position scroll;
	st_position scrolled;						// stores the value the map scrolled in this cycle. used for character movement control (it should move taking the scroll in account)
	std::list<object> object_list;
	bool wall_scroll_lock[MAP_W];
	float bg1_scroll;
	float bg2_scroll;
	graphicsLib_gSurface bg1_surface;
	graphicsLib_gSurface bg2_surface;
	short _platform_leave_counter;

};



#endif // CLASSMAP_H
