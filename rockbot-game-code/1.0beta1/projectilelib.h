#ifndef PROJECTILELIB_H
#define PROJECTILELIB_H

#include "st_common.h"
#include "defines.h"
#include <string>
#include "timerlib.h"
#include "graphicslib.h"

class classMap; // advance declaration

extern graphicsLib graphLib;

extern timerLib timer;

struct quake_info {
	int timer;
	short int counter1;
	short int counter2;
	quake_info() {
		timer = 0;
		counter1 = 0;
		counter2 = 0;
	}
};

class projectile
{
public:
	projectile(int projectile_id, int set_direction, int set_speed, st_position set_position, classMap *set_map, int damage, short int move_type);
	~projectile();
	st_size move();
	void draw();
	void set_map(classMap *set_map);
	bool check_colision(st_rectangle enemy_pos, st_position pos_inc);
    int get_damage();
	int get_direction();
	void reflect();
	short int get_move_type();
	void set_y(int sety);
	void consume_projectile();

private:
	void load_graphic(std::string filename);

public:
	bool is_finished;
	bool is_reflected;

private:
    int _id;
    st_size _size;
    unsigned int animation_pos;
    unsigned int animation_timer;
    unsigned int animation_speed;
	int direction;
	int speed;
	unsigned int move_timer;
	int move_delay;
	short int type; // type of weapon
	short int trajectory; // type of trajectory
	short int status; // status of animation (going/returning, etc)
	short int vertical_direction; // used in diagonal movement
	graphicsLib_gSurface graphic_surface; // graphic
    short int _damage;
	short int _move_type;

	st_position position;
	st_position realPosition;
	// used for trajectory calculations
	st_position position0;
	st_position position1;
	st_position map_position;
	classMap *map;

	bool diagonal_flag; // used to control diagonal shot angle
	short int _max_frames; // number of frames for the projectile
	float angle;
	short int radius;

	// used for quake effect
	quake_info _quake_info;

	// general effect timer
	int _effect_timer;
	short _effect_n; // used to control steps
	short _points; // duration

};
#endif // PROJECTILELIB_H
