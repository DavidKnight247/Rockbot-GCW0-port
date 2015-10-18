#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <map>
#include "graphicslib.h"

class classMap; // forward declaration

class object
{
public:
	object(int id, struct format_v2_0::file_object temp_obj); // game object constructor
	object(int id, object temp_obj, classMap *set_map, st_position map_pos); // map object constructor
	void show();
	void move();
	void execute();
	st_position get_position();
	st_size get_size();
	short int get_type();
	int get_direction();
	void set_direction(int);
	int get_distance();
	void set_distance(int);
	void set_timer(int);
	bool finished();
	void set_finished(bool is_finished);
	void invert_direction_x();
	void invert_direction_y();
	void set_state(short obj_state);
	short get_state();

private:
	void add_graphic();
	void gravity();
	bool test_change_position(short int xinc, short int yinc);
	void check_player_move(int xinc, int yinc); // if player uses this as platform, move him

private:
	unsigned int id;
	std::string name;
	std::string graphic_filename;												// graphic file used on it
	short int type;
	unsigned int obj_timer;														// used as time for disapearing block
	int speed;																	// used as speed for moving platform
	int limit;																	// used as range (pixels) for moving platform, visible time for disapearing block
	int direction;																// used to check if moving away from oiginalpoint or moving to it
	int distance;
	int framesize_w;
	int framesize_h;
	int frame;																	// indicates what is the used frame
	struct st_position start_point;
	struct st_position position;
	static std::map<std::string, graphicsLib_gSurface> objects_sprite_list;
	classMap *map;																// reference to the map this object is in
	bool _finished;
	short _state;
};

#endif // OBJECT_H
