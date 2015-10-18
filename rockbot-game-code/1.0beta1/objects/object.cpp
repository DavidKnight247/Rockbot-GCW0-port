#include "object.h"
#include "file/format.h"
#include "classmap.h"
#include "st_common.h"

extern graphicsLib graphLib;
extern std::string FILEPATH;

#include "timerlib.h"
extern timerLib timer;

#include "graphicslib.h"
extern graphicsLib graphLib;

std::map<std::string, graphicsLib_gSurface> object::objects_sprite_list;

#define GRAVITY_SPEED 4

#include "game.h"
extern game gameControl;


// constructor for map_object
/*
object::object(object game_obj)
{
}
*/



// constructor for game_object


object::object(int id, struct format_v2_0::file_object temp_obj) : _finished(false), _state(0)
{
	map = NULL;
	id = id;
	name = std::string(temp_obj.name);
	add_graphic();
	graphic_filename = std::string(temp_obj.graphic_filename);
	type = temp_obj.type;
	obj_timer = temp_obj.timer;
	speed = temp_obj.speed;
	limit = temp_obj.limit;
	direction = 0;
	distance = 0;
	framesize_w = temp_obj.size.width;
	framesize_h = temp_obj.size.heigth;
	frame = 0;
	start_point.x = -1;
	start_point.y = -1;
	position.x = -1;
	position.y = -1;
	std::string complete_filename(FILEPATH + "data/images/sprites/objects/" + graphic_filename);
	//std::cout << "object::object - name: " << name << ", filename: " << complete_filename << std::endl;

	graphLib.surfaceFromFile(complete_filename, &objects_sprite_list.find(name)->second);

	if (limit == 0 && (type == OBJ_MOVING_PLATFORM_LEFTRIGHT || type == OBJ_MOVING_PLATFORM_UPDOWN)) {
		limit = 48;
	}

}

// constructor for map_object
object::object(int id, struct object temp_obj, classMap *set_map, st_position map_pos) : _finished(false), _state(0)
{
	map = set_map;
	id = id;
	name = std::string(temp_obj.name);
	add_graphic();
	graphic_filename = temp_obj.graphic_filename;
	type = temp_obj.type;
	obj_timer = temp_obj.obj_timer;
	speed = temp_obj.speed;
	limit = temp_obj.limit;
	direction = 0;
	distance = 0;
	framesize_w = temp_obj.framesize_w;
	framesize_h = temp_obj.framesize_h;
	frame = 0;
	start_point.x = map_pos.x*TILESIZE;
	start_point.y = map_pos.y*TILESIZE;
	position.x = map_pos.x*TILESIZE;
	position.y = map_pos.y*TILESIZE;
	//std::string complete_filename(FILEPATH + graphic_filename);
	if (limit == 0 && (type == OBJ_MOVING_PLATFORM_LEFTRIGHT || type == OBJ_MOVING_PLATFORM_UPDOWN)) {
		limit = 48;
	}
}


// ********************************************************************************************** //
// adds an entry into object_graphics_list map, if needed                                      //
// ********************************************************************************************** //
void object::add_graphic()
{
	std::map<std::string, graphicsLib_gSurface>::iterator it;
	graphicsLib_gSurface temp_sprite;
	temp_sprite.gSurface = NULL;
	const std::string temp_name(name);

	it = objects_sprite_list.find(name);
	if (it == objects_sprite_list.end()) { // there is no graphic with this key yet, add it
		objects_sprite_list.insert(std::pair<std::string, graphicsLib_gSurface>(temp_name, temp_sprite));
		//std::cout << "2. object::object - adding graphic entry for object '" << name << "', list.size: " << objects_sprite_list.size() << std::endl;
	}
}

void object::gravity()
{
	// non-falling object types
	if (type == OBJ_MOVING_PLATFORM_UPDOWN || type == OBJ_MOVING_PLATFORM_LEFTRIGHT || type == OBJ_DISAPPEARING_BLOCK || type == OBJ_FALL_PLATFORM || type == OBJ_ITEM_FLY || type == OBJ_FLY_PLATFORM) {
		return;
	}
	for (int i=GRAVITY_SPEED; i>0; i--) {
		if (test_change_position(0, i) == true) {
			position.y += i;
			check_player_move(0, i);
			break;
		}
	}
	if (position.y > RES_H) {
		_finished = true;
	}
}

bool object::test_change_position(short xinc, short yinc)
{
	if (map == NULL) {
		return false;
	}
	if (xinc == 0 && yinc == 0) {
		return true;
	}

	short p1 = map->getMapPointLock(st_position((position.x+2+xinc)/TILESIZE, (position.y+yinc+framesize_h)/TILESIZE));
	short p2 = map->getMapPointLock(st_position((position.x+framesize_w-2+xinc)/TILESIZE, (position.y+yinc+framesize_h)/TILESIZE));
	//std::cout << "object::test_change_position - p1: " << p1 << ", p2: " << p2 << std::endl;
	if ((p1 == TERRAIN_UNBLOCKED ||  p1 == TERRAIN_WATER) && (p2 ==TERRAIN_UNBLOCKED ||  p2 == TERRAIN_WATER)) {
		return true;
	}
	return false;
}

void object::check_player_move(int xinc, int yinc)
{
	//std::cout << "object::check_player_move::START - p.platform: " << gameControl.get_player_platform() << ", this: " << this << std::endl;
	if (xinc == 0 && yinc == 0) {
		std::cout << "object::check_player_move::LEAVE" << std::endl;
		return;
	}
	if (gameControl.get_player_platform() == this) {
		//std::cout << "************* object::check_player_move - MOVE xinc: " << xinc << ", yinc: " << yinc << " **************" << std::endl;
		gameControl.change_player_position(xinc, yinc, 0);
	}
}



// ********************************************************************************************** //
// adds an entry into object_graphics_list map, if needed                                      //
// ********************************************************************************************** //
void object::show()
{
	st_rectangle graphic_origin;
	st_position graphic_destiny;

	if (map == NULL) {
		return;
	}
	//printf("********* check draw object, x: %d, y: %d\n", position.x, position.y);
	if (type == OBJ_DISAPPEARING_BLOCK && direction == ANIM_DIRECTION_RIGHT) {
		//printf("********* show_object_sprites - hidden object\n");
		return;
	}

	// checks if the Object is near the screen to show it
	if (position.x+16 >= abs(map->getMapScrolling().x) && position.x-16 <= abs(map->getMapScrolling().x)+RES_W) {
		// animation
		if (framesize_w * 2 >= (objects_sprite_list.find(name)->second).width)  {
			if (frame <= 0 || frame > ((objects_sprite_list.find(name)->second).width/framesize_w)-1) {
				frame = 0;
			}
			graphic_origin.x = frame * framesize_w;
			if (obj_timer < timer.getTimer()) {
				frame++;
				obj_timer = timer.getTimer()+400;
			}
		} else {
			graphic_origin.x = 0;
		}
		// direction
		//printf(">> sprite->h: %d, framesize_h*2: %d <<\n", sprite->h, (framesize_h*2));
		if (framesize_h*2 <= (objects_sprite_list.find(name)->second).height)  {
			if (direction != ANIM_DIRECTION_RIGHT) {
				graphic_origin.y = 0;
			} else {
				graphic_origin.y = framesize_h;
			}
		} else {
			graphic_origin.y = 0;
		}


		graphic_origin.w = framesize_w;
		graphic_origin.h = framesize_h;

		// parte que vai ser colada
		graphic_destiny.x = position.x - map->getMapScrolling().x;
		graphic_destiny.y = position.y + map->getMapScrolling().y;
		//std::cout << "searching for graphic '" << name << "'" << std::endl;
		//std::cout << "object::show - map->getMapScrolling().x: " << map->getMapScrolling().x << ", map->getMapScrolling().y: " << map->getMapScrolling().y << ", position.y: " << position.y << ", graphic_destiny.x: " << graphic_destiny.x << ", graphic_destiny.y: " << graphic_destiny.y << std::endl;

		std::map<std::string, graphicsLib_gSurface>::iterator it;
		it = objects_sprite_list.find(name);
		if (it != objects_sprite_list.end()) { // there is no graphic with this key yet, add it
			//graphLib.copyArea(graphic_origin, graphic_destiny, &it->second, &graphLib.gameScreen);
			graphLib.copyArea(st_rectangle(graphic_origin.x, graphic_origin.y, graphic_origin.w, graphic_origin.h), st_position(graphic_destiny.x, graphic_destiny.y), &it->second, &graphLib.gameScreen);
			//graphLib.copyArea(st_position(graphic_destiny.x, graphic_destiny.y), &it->second, &graphLib.gameScreen);
		}
	// remove item that is out of vision
	} else if (type == OBJ_ITEM_FLY || type == OBJ_ITEM_JUMP) {
		printf(">> remove item <<\n");
		type = -1;
	}
}

void object::move()
{
	//std::cout << "object::move::START" << std::endl;
	if (type == OBJ_MOVING_PLATFORM_LEFTRIGHT) {
		if (distance > limit) {
			invert_direction_x();
			distance = 0;
		}
		int xinc=0;
		if (direction == ANIM_DIRECTION_LEFT) {
			xinc = -2;
		} else {
			xinc = 2;
		}
		bool can_move = test_change_position(xinc, 0);
		//std::cout << "object::move::OBJ_MOVING_PLATFORM_LEFTRIGHT - xinc: " << xinc << ", can_move: " << can_move << ", distance: " << distance << ", limit: " << limit << std::endl;
		if (can_move) {
			position.x += xinc;
			check_player_move(xinc, 0);
			distance += abs(xinc);
		} else {
			invert_direction_x();
			distance = 0;
		}
	} else if (type == OBJ_MOVING_PLATFORM_UPDOWN) {
		if (distance > limit) {
			//std::cout << "OBJ_MOVING_PLATFORM_UPDOWN - limit reached - distance: " << distance << ", limit: " << limit << ", invert" << std::endl;
			invert_direction_y();
			distance = 0;
		}
		int yinc=0;
		if (direction == ANIM_DIRECTION_UP) {
			yinc = -2;
		} else {
			yinc = 2;
		}
		bool can_move = test_change_position(0, yinc);
		//std::cout << "object::move::OBJ_MOVING_PLATFORM_LEFTRIGHT - xinc: " << xinc << ", can_move: " << can_move << ", distance: " << distance << ", limit: " << limit << std::endl;
		if (can_move) {
			position.y += yinc;
			check_player_move(0, yinc);
			distance += abs(yinc);
		} else {
			std::cout << "1. OBJ_MOVING_PLATFORM_UPDOWN - colision, invert_direction_y - direction: " << direction << std::endl;
			invert_direction_y();
			std::cout << "2. OBJ_MOVING_PLATFORM_UPDOWN - colision, invert_direction_y - direction: " << direction << std::endl;
			distance = 0;
		}
	} else if (type == OBJ_FALL_PLATFORM) {
		if (_state == 1) {
			int yinc = 3;
			position.y += yinc;
			distance += abs(yinc);
		} else if (_state == 2) {
			int yinc = -1;
			position.y += yinc;
			distance -= abs(yinc);
			if (distance == 0) {
				_state = 0;
			}
		}
	} else if (type == OBJ_FLY_PLATFORM) {
		if (_state == 1) {
			int yinc = -3;
			position.y += yinc;
			check_player_move(0, yinc);
			distance += abs(yinc);
		} else if (_state == 2) {
			int yinc = 1;
			position.y += yinc;
			check_player_move(0, yinc);
			distance -= abs(yinc);
			if (distance == 0) {
				_state = 0;
			}
		}
	}
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void object::execute()
{
	move();
	gravity();
	show();
	/// @TODO: finish items
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_position object::get_position()
{
	return position;
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
st_size object::get_size()
{
	return st_size(framesize_w, framesize_h);
}


// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
short int object::get_type()
{
	return type;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
int object::get_direction()
{
	return direction;
}

// ********************************************************************************************** //
//                                                                                                //
// ********************************************************************************************** //
void object::set_direction(int new_dir)
{
	direction = new_dir;
}

int object::get_distance()
{
	return distance;
}



void object::set_distance(int new_dist)
{
	distance = new_dist;
}


void object::set_timer(int new_timer)
{
	obj_timer = new_timer;
}

bool object::finished()
{
	return _finished;
}

void object::set_finished(bool is_finished)
{
	_finished = is_finished;
}

void object::invert_direction_x()
{
	if (direction == ANIM_DIRECTION_LEFT) {
		//std::cout << "invert left->right" << std::endl;
		direction = ANIM_DIRECTION_RIGHT;
	} else {
		//std::cout << "invert right->left" << std::endl;
		direction = ANIM_DIRECTION_LEFT;
	}
}

void object::invert_direction_y() {
	if (direction == ANIM_DIRECTION_UP) {
		//std::cout << "invert up->down" << std::endl;
		direction = ANIM_DIRECTION_DOWN;
	} else {
		//std::cout << "invert down->up" << std::endl;
		direction = ANIM_DIRECTION_UP;
	}
}

void object::set_state(short obj_state)
{
	_state = obj_state;
	//std::cout << "********* object::set_state::state: " << _state << std::endl;
}

short object::get_state()
{
	return _state;
}

